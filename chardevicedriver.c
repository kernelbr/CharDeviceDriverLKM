#include <linux/module.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "chardevicedriver.h"


static unsigned int cdd_major = CDD_MAJOR;
static unsigned int cdd_minor = CDD_MINOR;
static unsigned int cdd_nr_devs = CDD_NR_DEVS;
static char cdd_byte;
struct cdev cdd_cdev;

module_param(cdd_major, uint, S_IRUGO);
module_param(cdd_minor, uint, S_IRUGO);
module_param(cdd_nr_devs, uint, S_IRUGO);

static int cdd_open(struct inode *, struct file *);
static int cdd_release(struct inode *, struct file *);
static ssize_t cdd_write(struct file *, const char *, size_t, loff_t *);
static ssize_t cdd_read(struct file *, char *, size_t, loff_t *);


static struct file_operations cdd_fops =
{
	.owner		= THIS_MODULE,
	.read		= cdd_read,
	.write		= cdd_write,
	.open		= cdd_open,
	.release	= cdd_release
};

static void
cdd_setup_cdev(struct cdev *dev)
{
	int err, devno = MKDEV(cdd_major, cdd_minor);
	cdev_init(dev, &cdd_fops);
	dev->owner = THIS_MODULE;
	dev->ops = &cdd_fops;
	err = cdev_add(dev, devno, 1);
	if (err)
		printk(KERN_WARNING "cdd: can't add cdd");
}

static int __init
cdd_init(void)
{
	int r;
	dev_t dev;

	if (cdd_major) {
		dev = MKDEV(cdd_major, cdd_minor);
		r = register_chrdev_region(dev, cdd_nr_devs, "cdd");
	} else {
		r = alloc_chrdev_region(&dev, cdd_minor, cdd_nr_devs, "cdd");
		cdd_major = MAJOR(dev);
	}

	if (r < 0) {
		printk(KERN_WARNING "cdd: can't get major %d\n", cdd_major);
		return r;
	}
	
	cdd_setup_cdev(&cdd_cdev);

	return r;
}

static void __exit
cdd_cleanup(void)
{
	dev_t dev = MKDEV(cdd_major, cdd_minor);
	cdev_del(&cdd_cdev);
        unregister_chrdev_region(dev, cdd_nr_devs);
	printk(KERN_INFO "cdd: exiting module\n");
}

static int
cdd_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "cdd: opening device\n");
	return 0;
}

static int
cdd_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "cdd: closing device\n");
	return 0;
}

static ssize_t
cdd_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_INFO "cdd: reading from device\n");
	copy_to_user(buff, &cdd_byte, 1);
	if (!*off)
 		return *off += 1;
	return 0;
}

static ssize_t
cdd_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_INFO "cdd: writing on device\n");
	copy_from_user(&cdd_byte, buff, 1);
	return 1;
}

module_init(cdd_init);
module_exit(cdd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel BR team");
MODULE_DESCRIPTION("An example of Linux Char Device Driver");
