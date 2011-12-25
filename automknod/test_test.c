#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/utsname.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

static struct class *usb_class;
static struct device *dev;
static const char shortname [] = "test_usb";
static dev_t test_usb_devno;


static int test_sysfs_init_module(void)
{
	int    ret;
	printk( KERN_DEBUG "Module test_sysfs init\n" );
	usb_class = class_create(THIS_MODULE, shortname);
	if (IS_ERR(usb_class))
	{
	printk( KERN_DEBUG "class_create error\n" );
	return -1;
}

ret=alloc_chrdev_region(&test_usb_devno, 0, 1, shortname);
	if (ret)
	{
	printk( KERN_DEBUG "alloc_chrdev_region error\n" );
	class_destroy(usb_class);
	return ret;
	}

	dev=device_create(usb_class, NULL, test_usb_devno, NULL,shortname);
	return 0;
}

static void test_sysfs_exit_module(void)
{
	/* Remove sysfs files */
	device_destroy(usb_class, test_usb_devno);
	unregister_chrdev_region(test_usb_devno, 1);
	class_destroy(usb_class);
	printk( KERN_DEBUG "Module test_sysfs exit\n" );
}

module_init(test_sysfs_init_module);
module_exit(test_sysfs_exit_module);
