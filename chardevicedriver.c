#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static char byte;

static int cdd_open(struct inode *, struct file *);
static int cdd_release(struct inode *, struct file *);
static ssize_t cdd_write(struct file *, const char *, size_t, loff_t *);
static ssize_t cdd_read(struct file *, char *, size_t, loff_t *);


static struct file_operations fops =
{
	read:	cdd_read,
	write:	cdd_write,
	open:	cdd_open,
	release:cdd_release,
};


static int __init cdd_init(void)
{
	int r = register_chrdev(66, "cdd", &fops);

	if (r < 0)
		printk(KERN_ALERT "cdd: device registration failed\n");
	else
		printk(KERN_INFO "cdd: device registered\n");

	return r;
}

static void __exit cdd_cleanup(void)
{
	unregister_chrdev(66, "cdd");
	printk(KERN_INFO "cdd: exiting module\n");
}

static int cdd_open(struct inode *inode, struct file *f)
{
	printk(KERN_INFO "cdd: opening device\n");
	return 0;
}

static int cdd_release(struct inode *inode, struct file *f)
{
	printk(KERN_INFO "cdd: closing device\n");
	return 0;
}

static ssize_t cdd_read(struct file *f, char *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "cdd: reading from device\n");

	put_user(byte, buf);
	if (!*off)	return *off += 1;
	else		return 0;
}

static ssize_t cdd_write(struct file *f, const char *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "cdd: writing on device\n");

	byte = *buf;
	return 1;
}

module_init(cdd_init);
module_exit(cdd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel BR team");
MODULE_DESCRIPTION("char device driver module");

