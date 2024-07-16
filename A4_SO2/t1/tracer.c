// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * Author: Cristian Simache <Cristiansimache@gmail.com>
 */

#include "./tracer.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/kprobes.h>
#include <linux/hashtable.h>
#include <linux/miscdevice.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>


#ifndef BUFSIZ
#define BUFSIZ     4096
#endif

#define MAXACTIVE 64

// hashtable of struct tracked_info
DECLARE_HASHTABLE(trackers, 8);
// lock used for operations on the list and elements in it
// the write lock is for adding and removeing elements to the list
// the read lock is held as long as any field of the element is used
// in the case of multiple writes to a field the read lock is suficient
//	pid is set at creation
//	memory_allocations has it's own lock
//	the rest are atomic_t
DEFINE_RWLOCK(trackers_rw_lock);

struct tracked_info {
	pid_t pid;
	struct hlist_node node;

	// hashtable of malloc_call_info
	DECLARE_HASHTABLE(memory_allocations, 8);
	spinlock_t allocations_lock;

	atomic_t kmalloc_call_count;
	atomic_t allocated_memory;

	atomic_t kfree_call_count;
	atomic_t freed_memory;

	atomic_t schedule_call_count;

	atomic_t up_call_count;
	atomic_t down_interruptible_call_count;

	atomic_t mutex_lock_call_count;
	atomic_t mutex_unlock_call_count;
};

struct malloc_call_info {
	struct hlist_node node;

	void *addr;
	size_t requested_memory_size;
};

static struct proc_dir_entry *proc_tracer_read;

static inline struct tracked_info *get_process_info(pid_t pid)
{
	struct tracked_info *p;

	hash_for_each_possible(trackers, p,  node, pid)
		if (p->pid == pid)
			return p;

	return NULL;
}

static int kmalloc_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct malloc_call_info *data = (struct malloc_call_info *) a->data;

	data->requested_memory_size = regs_get_kernel_argument(regs, 0);
	return 0;
}

static int kmalloc_probe_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *info;
	struct malloc_call_info *new_call_info;
	struct malloc_call_info *data = (struct malloc_call_info *) a->data;

	if (!regs_return_value(regs))
		return 0;

	read_lock(&trackers_rw_lock);
	info = get_process_info(current->pid);
	if (!info) {
		read_unlock(&trackers_rw_lock);
		return 0;
	}

	atomic_add(data->requested_memory_size, &info->allocated_memory);
	atomic_inc(&info->kmalloc_call_count);

	new_call_info = kmalloc(sizeof(struct malloc_call_info), GFP_ATOMIC);
	new_call_info->addr = (void *)regs_return_value(regs);
	new_call_info->requested_memory_size = data->requested_memory_size;

	spin_lock(&info->allocations_lock);
	hash_add(info->memory_allocations, &(new_call_info->node), (uintptr_t)new_call_info->addr);
	spin_unlock(&info->allocations_lock);

	read_unlock(&trackers_rw_lock);

	return 0;
}

static inline struct malloc_call_info *get_allocation_info(struct tracked_info *target, void *addr)
{
	struct malloc_call_info *p;

	hash_for_each_possible(target->memory_allocations, p,  node, (uintptr_t)addr)
		if (p->addr == addr)
			return p;

	return NULL;
}


static int kfree_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;
	struct malloc_call_info *allocation_info;

	if (!regs_get_kernel_argument(regs, 0))
		return 0;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);
	if (!process_info) {
		read_unlock(&trackers_rw_lock);
		return 0;
	}

	spin_lock(&process_info->allocations_lock);
	allocation_info = get_allocation_info(process_info, (void *)regs_get_kernel_argument(regs, 0));
	if (!allocation_info) {
		spin_unlock(&process_info->allocations_lock);
		read_unlock(&trackers_rw_lock);
		return 0;
	}

	hash_del(&allocation_info->node);
	spin_unlock(&process_info->allocations_lock);

	atomic_add(allocation_info->requested_memory_size, &process_info->freed_memory);
	atomic_inc(&process_info->kfree_call_count);

	read_unlock(&trackers_rw_lock);

	kfree(allocation_info);
	return 0;
}

static int schedule_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);

	if (process_info)
		atomic_inc(&process_info->schedule_call_count);

	read_unlock(&trackers_rw_lock);
	return 0;
}

static int up_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);

	if (process_info)
		atomic_inc(&process_info->up_call_count);

	read_unlock(&trackers_rw_lock);
	return 0;
}

static int down_interruptible_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);

	if (process_info)
		atomic_inc(&process_info->down_interruptible_call_count);

	read_unlock(&trackers_rw_lock);
	return 0;
}

static int mutex_lock_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);

	if (process_info)
		atomic_inc(&process_info->mutex_lock_call_count);

	read_unlock(&trackers_rw_lock);
	return 0;
}

static int mutex_unlock_probe_entry_handler(struct kretprobe_instance *a, struct pt_regs *regs)
{
	struct tracked_info *process_info;

	read_lock(&trackers_rw_lock);
	process_info = get_process_info(current->pid);

	if (process_info)
		atomic_inc(&process_info->mutex_unlock_call_count);

	read_unlock(&trackers_rw_lock);
	return 0;
}

static long tracer_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pid_t pid = (pid_t) arg;
	struct tracked_info *p;
	struct hlist_node *tmp;
	int bkt;

	if (cmd == TRACER_ADD_PROCESS) {
		struct tracked_info *new_node = kmalloc(sizeof(struct tracked_info), GFP_KERNEL);

		memset(new_node, 0, sizeof(*new_node));
		hash_init(new_node->memory_allocations);
		spin_lock_init(&new_node->allocations_lock);

		new_node->pid = pid;

		write_lock(&trackers_rw_lock);
		hash_for_each_possible_safe(trackers, p, tmp, node, pid) {
			if (p->pid == pid)
				goto add_overload;
		}


		hash_add(trackers, &(new_node->node), pid);
		write_unlock(&trackers_rw_lock);

		return 0;
add_overload:
		write_unlock(&trackers_rw_lock);
		kfree(new_node);

		return -1;
	} else if (cmd == TRACER_REMOVE_PROCESS) {
		struct malloc_call_info *allocation_info;

		write_lock(&trackers_rw_lock);
		hash_for_each_possible_safe(trackers, p, tmp, node, pid) {
			if (p->pid != pid)
				continue;

			hash_del(&p->node);
			break;
		}

		write_unlock(&trackers_rw_lock);

		hash_for_each_safe(p->memory_allocations, bkt, tmp, allocation_info, node) {
			hash_del(&allocation_info->node);
			kfree(allocation_info);
		}

		kfree(p);
	} else
		return -1;
	return 0;
}

static int tracer_proc_read(struct seq_file *file, void *v)
{
	int bkt;
	struct tracked_info *p;

	seq_puts(file, "PID\tkmalloc\tkfree\tkmalloc_mem\tkfree_mem\tsched\tup\tdown\tlock\tunlock\n");

	read_lock(&trackers_rw_lock);
	hash_for_each(trackers, bkt, p, node)
		seq_printf(file, "%d\t%d\t%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\t%d\n",
				p->pid,
				atomic_read(&p->kmalloc_call_count),
				atomic_read(&p->kfree_call_count),
				atomic_read(&p->allocated_memory),
				atomic_read(&p->freed_memory),
				atomic_read(&p->schedule_call_count),
				atomic_read(&p->up_call_count),
				atomic_read(&p->down_interruptible_call_count),
				atomic_read(&p->mutex_lock_call_count),
				atomic_read(&p->mutex_unlock_call_count)
		);
	read_unlock(&trackers_rw_lock);

	return 0;
}

static int tracer_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, tracer_proc_read, NULL);
}

static int tracer_dev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int tracer_dev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct kretprobe kmalloc_probe = {
	.kp = { .symbol_name = "__kmalloc", },
	.entry_handler = kmalloc_probe_entry_handler, /* entry handler */
	.handler = kmalloc_probe_handler, /* return probe handler */
	.maxactive = MAXACTIVE,
	.data_size = sizeof(struct malloc_call_info),
};

static struct kretprobe kfree_probe = {
	.kp = { .symbol_name = "kfree", },
	.entry_handler = kfree_probe_entry_handler,
	.maxactive = MAXACTIVE
};

static struct kretprobe schedule_probe = {
	.kp = { .symbol_name = "schedule", },
	.entry_handler = schedule_probe_entry_handler,
	.maxactive = MAXACTIVE
};

static struct kretprobe up_probe = {
	.kp = { .symbol_name = "up", },
	.entry_handler = up_probe_entry_handler,
	.maxactive = MAXACTIVE
};

static struct kretprobe down_interruptible_probe = {
	.kp = { .symbol_name = "down_interruptible", },
	.entry_handler = down_interruptible_probe_entry_handler,
	.maxactive = MAXACTIVE
};

static struct kretprobe mutex_lock_probe = {
	.kp = { .symbol_name = "mutex_lock_nested", },
	.entry_handler = mutex_lock_probe_entry_handler,
	.maxactive = MAXACTIVE
};

static struct kretprobe mutex_unlock_probe = {
	.kp = { .symbol_name = "mutex_unlock", },
	.entry_handler = mutex_unlock_probe_entry_handler,
	.maxactive = MAXACTIVE
};


static const struct file_operations fops_dev = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl = tracer_dev_ioctl,
	.open = tracer_dev_open,
	.release = tracer_dev_release,
};

static const struct proc_ops fops_proc_read = {
	.proc_open = tracer_proc_open,
	.proc_read = seq_read,
	.proc_release = single_release
};

struct miscdevice tracer_misc_dev = {
	.minor = TRACER_DEV_MINOR,
	.name = TRACER_DEV_NAME,
	.fops = &fops_dev,
};

static int tracer_init(void)
{
	int err;

	hash_init(trackers);

	err = misc_register(&tracer_misc_dev);
	if (err)
		return err;

	proc_tracer_read = proc_create(TRACER_DEV_NAME, 0000, NULL, &fops_proc_read);
	if (!proc_tracer_read)
		goto tracer_misc_cleanup;

	err = register_kretprobe(&kmalloc_probe);
	if (err)
		goto tracer_proc_cleanup;

	err = register_kretprobe(&kfree_probe);
	if (err)
		goto tracer_probe_kmalloc_cleanup;

	err = register_kretprobe(&schedule_probe);
	if (err)
		goto tracer_probe_kfree_cleanup;

	err = register_kretprobe(&up_probe);
	if (err)
		goto tracer_probe_schedule_cleanup;

	err = register_kretprobe(&down_interruptible_probe);
	if (err)
		goto tracer_probe_up_cleanup;

	err = register_kretprobe(&mutex_lock_probe);
	if (err)
		goto tracer_probe_down_interruptible_cleanup;

	err = register_kretprobe(&mutex_unlock_probe);
	if (err)
		goto tracer_probe_mutex_lock_cleanup;

	return 0;

tracer_probe_mutex_lock_cleanup:
	unregister_kretprobe(&mutex_lock_probe);

tracer_probe_down_interruptible_cleanup:
	unregister_kretprobe(&down_interruptible_probe);

tracer_probe_up_cleanup:
	unregister_kretprobe(&up_probe);

tracer_probe_schedule_cleanup:
	unregister_kretprobe(&schedule_probe);

tracer_probe_kfree_cleanup:
	unregister_kretprobe(&kfree_probe);

tracer_probe_kmalloc_cleanup:
	unregister_kretprobe(&kmalloc_probe);

tracer_proc_cleanup:
	proc_remove(proc_tracer_read);

tracer_misc_cleanup:
	misc_deregister(&tracer_misc_dev);

	return err;
}

static void tracer_exit(void)
{
	unregister_kretprobe(&mutex_unlock_probe);
	unregister_kretprobe(&mutex_lock_probe);
	unregister_kretprobe(&schedule_probe);
	unregister_kretprobe(&down_interruptible_probe);
	unregister_kretprobe(&up_probe);
	unregister_kretprobe(&kfree_probe);
	unregister_kretprobe(&kmalloc_probe);

	proc_remove(proc_tracer_read);
	misc_deregister(&tracer_misc_dev);
}

module_init(tracer_init);
module_exit(tracer_exit);


MODULE_DESCRIPTION("Linux kernel tracer API");
MODULE_AUTHOR("Cristian SImache <Cristiansimache@gmail.com");
MODULE_LICENSE("GPL v2");
