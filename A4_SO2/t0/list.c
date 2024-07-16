// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * Author: Cristian Simache <Cristiansimache@gmail.com>
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

#define PROCFS_MAX_SIZE		512

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

static LIST_HEAD(list_head);

struct list_node {
	char *name;
	struct list_head list;
};

static DEFINE_RWLOCK(lock);

static int list_proc_show(struct seq_file *m, void *v)
{
	struct list_head *p;

	read_lock(&lock);
	list_for_each(p, &list_head)
		seq_puts(m, list_entry(p, struct list_node, list)->name);
	read_unlock(&lock);

	return 0;
}

static int list_read_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static struct list_node *create_node(char *name)
{
	struct list_node *new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);

	if (!new_node)
		return NULL;

	new_node->name = kmalloc(strlen(name) + 1, GFP_KERNEL);
	if (!new_node->name) {
		kfree(new_node);
		return NULL;
	}

	return new_node;
}

static void command_addf(char *name)
{
	struct list_node *new_node = create_node(name);

	if (!new_node)
		return;

	strcpy(new_node->name, name);

	write_lock(&lock);
	list_add(&new_node->list, &list_head);
	write_unlock(&lock);
}

static void command_adde(char *name)
{
	struct list_node *new_node = create_node(name);

	if (!new_node)
		return;

	strcpy(new_node->name, name);

	write_lock(&lock);
	list_add_tail(&new_node->list, &list_head);
	write_unlock(&lock);
}


static void command_del(char *name, int del_all)
{
	struct list_head *p, *n;

	write_lock(&lock);
	list_for_each_safe(p, n, &list_head) {
		struct list_node *node = list_entry(p, struct list_node, list);

		if (!strcmp(name, node->name)) {
			list_del(p);
			kfree(node->name);
			kfree(node);

			if (!del_all)
				goto exit;
		}
	}
exit:
	write_unlock(&lock);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
			  size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;
	char *name;

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	/* local_buffer contains your command written in /proc/list/management
	 */
	name = local_buffer+5;

	// check name was provided
	if (local_buffer_size < 5 || !name[0])
		return 0;

	if (!strncmp(local_buffer, "addf ", 5))
		command_addf(name);
	else if (!strncmp(local_buffer, "adde ", 5))
		command_adde(name);
	else if (!strncmp(local_buffer, "delf ", 5))
		command_del(name, 0);
	else if (!strncmp(local_buffer, "dela ", 5))
		command_del(name, 1);

	return local_buffer_size;
}

static const struct proc_ops r_pops = {
	.proc_open		= list_read_open,
	.proc_read		= seq_read,
	.proc_release	= single_release,
};

static const struct proc_ops w_pops = {
	.proc_open		= list_write_open,
	.proc_write		= list_write,
	.proc_release	= single_release,
};

static int list_init(void)
{
	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
				     &r_pops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
				      &w_pops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");
MODULE_AUTHOR("Cristian Simache <Cristiansimache@gmail.com>");
MODULE_LICENSE("GPL v2");
