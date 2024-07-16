// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * Author: Cristian Simache <Cristiansimache@gmail.com>
 */

#include "./uart16550.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/kfifo.h>
#include <linux/interrupt.h>

#define UART_DEV_NAME "uart16550"
#define MODULE_NAME "uart16550"


static int major = 42;
module_param(major, int, 0000);

static int option = OPTION_BOTH;
module_param(option, int, 0000);

static int enabled_minors = UART16550_COM1_SELECTED | UART16550_COM2_SELECTED;


#define FIFO_BUF_SIZE 1024

#define COM1_ADDRESS 0x3f8
#define COM1_SIZE 8
#define COM2_ADDRESS 0x2f8
#define COM2_SIZE 8

#define IRQ_COM1 4
#define IRQ_COM2 3

#define UART16550_PORT_RXR	0
#define UART16550_PORT_TXR	0

#define UART16550_PORT_IER	1
#define UART16550_PORT_IIR	2

#define UART16550_PORT_LCR	3
#define UART16550_PORT_LSR	5

#define UART16550_PORT_FCR	2
#define UART16550_PORT_MCR	4
#define UART16550_PORT_MSR	5

#define UART16550_PORT_DLL	0
#define UART16550_PORT_DLH	0

#define set_DLAB(port_address) \
	outb(inb(port_address + UART16550_PORT_LCR) | 0b10000000, \
	     port_address + UART16550_PORT_LCR \
)

#define clear_DLAB(port_address) \
	outb(inb(port_address + UART16550_PORT_LCR) & 0b01111111, \
	     port_address + UART16550_PORT_LCR \
)

struct uart_device_data {
	struct cdev cdev;
	DECLARE_KFIFO(data_from_port, char, FIFO_BUF_SIZE);
	DECLARE_KFIFO(data_to_port, char, FIFO_BUF_SIZE);
	wait_queue_head_t wq;
	int port_address;
	int port_size;
	rwlock_t dlab_lock;
};

struct uart_device_data devs[MAX_NUMBER_DEVICES];


static irqreturn_t uart_interupt_handler(int irq_no, void *dev_id)
{
	struct uart_device_data *data = (struct uart_device_data *) dev_id;

	unsigned int iir = inb(data->port_address + UART16550_PORT_IIR);

	const int iir_code = (iir >> 1) & 0b111;
	char line_data;

	if ((iir & 0b11000000) != 0b11000000)
		pr_err("FIFO malfunction\n\tcode=%d\n", iir >> 6);

	if (iir_code == 0b110 || iir_code == 0b010) {
		read_lock(&data->dlab_lock);
		kfifo_put(&data->data_from_port,
			  inb(data->port_address + UART16550_PORT_RXR));
		read_unlock(&data->dlab_lock);
		wake_up(&data->wq);
	} else if (iir_code == 0b001) {
		// if the receiver doesn't take it's data ASAP
		// THEN this needs to be changed to checking modem status intr
		// no the case with a big FIFO_BUF_SIZE

		// exit is there is no more data to send
		if (!kfifo_get(&data->data_to_port, &line_data))
			return IRQ_HANDLED;

		read_lock(&data->dlab_lock);
		outb(line_data, data->port_address + UART16550_PORT_TXR);
		read_unlock(&data->dlab_lock);
		wake_up(&data->wq);
	} else if (iir_code == 0) {
		// trigered on handshake
		// can be ignored
		// read to reset interupt

		inb(data->port_address + UART16550_PORT_MSR);
	} else
		pr_err("Unhandled interupt %d\n", iir_code);

	return IRQ_HANDLED;
}

static int uart_open(struct inode *inode, struct file *file)
{
	struct uart_device_data *data = container_of(inode->i_cdev, struct uart_device_data, cdev);

	file->private_data = data;

	return 0;
}

static int uart_read(struct file *file, char __user *user_buffer,
			size_t size, loff_t *offset)
{
	struct uart_device_data *data = (struct uart_device_data *) file->private_data;
	int count = 0;

	if (kfifo_is_empty(&data->data_from_port))
		if (-ERESTARTSYS == wait_event_interruptible(data->wq, !kfifo_is_empty(&data->data_from_port)))
			return 0;

	kfifo_to_user(&data->data_from_port, user_buffer, size, &count);

	return count;
}

static int uart_write(struct file *file, const char __user *user_buffer,
			size_t size, loff_t *offset)
{
	struct uart_device_data *data = (struct uart_device_data *) file->private_data;

	int count = 0;

	if (kfifo_is_full(&data->data_to_port))
		if (-ERESTARTSYS == wait_event_interruptible(data->wq, !kfifo_is_full(&data->data_to_port)))
			return 0;

	kfifo_from_user(&data->data_to_port, user_buffer, size, &count);

	if (inb(data->port_address + UART16550_PORT_LSR) & 0b01000000) {
		// check if a transfer isn't in progress and start it
		char send_data;

		if (kfifo_get(&data->data_to_port, &send_data) == 1) {
			read_lock(&data->dlab_lock);
			// check we have data in fifo
			// shouldn't fail but jic
			outb(send_data, data->port_address + UART16550_PORT_TXR);
			read_unlock(&data->dlab_lock);
		} else
			pr_err("No data in fifo for new transfer\n\tport=%d\n",
				data->port_address);
	}

	return count;
}


static int check_line_baud(int baud)
{
	if (baud == UART16550_BAUD_9600)
		return 1;
	if (baud == UART16550_BAUD_1200)
		return 1;
	if (baud == UART16550_BAUD_2400)
		return 1;
	if (baud == UART16550_BAUD_4800)
		return 1;
	if (baud == UART16550_BAUD_19200)
		return 1;
	if (baud == UART16550_BAUD_38400)
		return 1;
	if (baud == UART16550_BAUD_56000)
		return 1;
	if (baud == UART16550_BAUD_115200)
		return 1;

	return 0;
}

static int check_line_stop(int stop)
{
	if (stop == UART16550_STOP_1)
		return 1;
	if (stop == UART16550_STOP_2)
		return 1;

	return 0;
}

static int check_line_par(int par)
{
	if (par == UART16550_PAR_NONE)
		return 1;
	if (par == UART16550_PAR_ODD)
		return 1;
	if (par == UART16550_PAR_EVEN)
		return 1;
	if (par == UART16550_PAR_STICK)
		return 1;

	return 0;
}

static int check_line_info(struct uart16550_line_info *info)
{
	if (!check_line_baud(info->baud))
		return 0;
	if (!check_line_stop(info->stop))
		return 0;
	if (!check_line_par(info->par))
		return 0;
	return 1;
}

static long uart_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct uart_device_data *data = (struct uart_device_data *) file->private_data;
	struct uart16550_line_info info;

	if (cmd != UART16550_IOCTL_SET_LINE)
		return -EINVAL;

	if (copy_from_user(&info, (void *)arg, sizeof(info)))
		return -EFAULT;

	if (!check_line_info(&info))
		return -EINVAL;

	// set baud rate
	write_lock_irq(&data->dlab_lock);
	set_DLAB(data->port_address);
	outb(info.baud & 0xFF,   data->port_address + UART16550_PORT_DLL);
	outb(info.baud >> 8, data->port_address + UART16550_PORT_DLH);
	clear_DLAB(data->port_address);
	write_unlock_irq(&data->dlab_lock);

	outb(info.par | info.stop | info.len, data->port_address + UART16550_PORT_LCR);

	return 0;
}

static const struct file_operations uart_fops = {
	.owner = THIS_MODULE,
	.open = uart_open,
	.read = uart_read,
	.write = uart_write,
	.unlocked_ioctl = uart_ioctl,
};

static inline int uart_init_com(int irq_no, int dev_idx,
				int port_address, int port_size,
				const char *region_name, int chrdev_minor)
{
	int baud = UART16550_BAUD_9600;
	int err = 0;
	struct uart_device_data *const dev = &devs[dev_idx];

	init_waitqueue_head(&dev->wq);

	INIT_KFIFO(dev->data_from_port);
	INIT_KFIFO(dev->data_to_port);

	rwlock_init(&dev->dlab_lock);

	dev->port_address = port_address;
	dev->port_size = port_size;

	err = register_chrdev_region(MKDEV(major, chrdev_minor), 1, UART_DEV_NAME);
	if (err)
		return 1;

	cdev_init(&dev->cdev, &uart_fops);
	err = cdev_add(&dev->cdev, MKDEV(major, chrdev_minor), 1);
	if (err)
		goto com_cdev_add_failed;

	if (!request_region(port_address, port_size, MODULE_NAME))
		goto com_region_failed;

	err = request_irq(irq_no, uart_interupt_handler, IRQF_SHARED, MODULE_NAME, dev);
	if (err)
		goto com_irq_request_failed;

	// init device
	set_DLAB(port_address);
	outb(baud >> 8,   port_address + UART16550_PORT_DLL);
	outb(baud & 0xFF, port_address + UART16550_PORT_DLH);
	outb(0x0C,   port_address + UART16550_PORT_DLL);
	outb(0x00, port_address + UART16550_PORT_DLH);

	clear_DLAB(port_address);

	outb(0b01000011, port_address + UART16550_PORT_LCR);
	outb(0b11000111, port_address + UART16550_PORT_FCR);
	outb(0b00000011, port_address + UART16550_PORT_MCR);

	// enable interupts
	outb((1<<0)|(1<<1), port_address + UART16550_PORT_IER);

	return 0;
com_irq_request_failed:
	release_region(port_address, port_size);

com_region_failed:
	cdev_del(&dev->cdev);

com_cdev_add_failed:
	unregister_chrdev_region(MKDEV(major, chrdev_minor), 1);

	return 1;
}

static int uart_init_com1(void)
{
	return uart_init_com(
		IRQ_COM1,
		0,
		COM1_ADDRESS,
		COM1_SIZE,
		"com1",
		0
	);
}

static int uart_init_com2(void)
{
	return uart_init_com(
		IRQ_COM2,
		1,
		COM2_ADDRESS,
		COM2_SIZE,
		"com2",
		1
	);
}

static inline void uart_cleanup(int irq_no, int dev_idx,
				int port_address, int port_size,
				int chrdev_minor)
{
	free_irq(irq_no, &devs[dev_idx]);
	release_region(port_address, port_size);
	cdev_del(&devs[dev_idx].cdev);
	unregister_chrdev_region(MKDEV(major, chrdev_minor), 1);
}

static inline void uart_cleanup_com1(void)
{
	uart_cleanup(
		IRQ_COM1,
		0,
		COM1_ADDRESS,
		COM1_SIZE,
		0
	);
}

static inline void uart_cleanup_com2(void)
{
	uart_cleanup(
		IRQ_COM2,
		1,
		COM2_ADDRESS,
		COM2_SIZE,
		1
	);
}

static int uart_init(void)
{
	pr_info("UART driver loaded with major=%d, option=%d\n", major, option);

	if (option == OPTION_BOTH)
		enabled_minors = UART16550_COM1_SELECTED | UART16550_COM2_SELECTED;
	else if (option == OPTION_COM1)
		enabled_minors = UART16550_COM1_SELECTED;
	else if (option == OPTION_COM2)
		enabled_minors = UART16550_COM2_SELECTED;
	else
		return -EBUSY;


	if (enabled_minors & UART16550_COM1_SELECTED) {
		if (uart_init_com1())
			return -EBUSY;
		pr_info("COM1: initialized\n");
	}

	if (enabled_minors & UART16550_COM2_SELECTED) {
		if (uart_init_com2())
			goto com2_init_failed;
		pr_info("COM2: initialized\n");
	}

	return 0;
com2_init_failed:
	uart_cleanup_com1();

	return -EBUSY;
}

static void uart_exit(void)
{
	if (enabled_minors & UART16550_COM1_SELECTED)
		uart_cleanup_com1();

	if (enabled_minors & UART16550_COM2_SELECTED)
		uart_cleanup_com2();

}

module_init(uart_init);
module_exit(uart_exit);


MODULE_DESCRIPTION("Linux kernel tracer API");
MODULE_AUTHOR("Cristian SImache <Cristiansimache@gmail.com");
MODULE_LICENSE("GPL v2");
