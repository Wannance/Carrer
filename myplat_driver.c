#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>

#define MYPLAT_MAJOR 222
#define MYPLAT_MINOR 0
#define MEM_SIZE	 1024

static dev_t myplat_devno;
static int myplat_major;
static struct class *cls;
static struct device *myplat_device;
static struct cdev *myplat_cdev;

static ssize_t myplat_read(struct file *fs, char __user *buf, size_t size, loff_t *len)
{
	printk(KERN_NOTICE "myplat_read\n");
	return 0;
}

static ssize_t myplat_write(struct file *fs, const char __user *buf, size_t size, loff_t *len)
{
	printk(KERN_NOTICE "myplat_write\n");
	return 0;
}

static struct file_operations myplat_ops = {
	.owner = THIS_MODULE,
	.read = myplat_read,
	.write = myplat_write,
};

// 注册设备
static int myplat_driver_register(void)
{
	int ret = 0;
	printk(KERN_NOTICE "myplat_driver_register\n");
	myplat_devno = MKDEV(myplat_major, 0);
	if (myplat_major) {
		ret = register_chrdev_region(myplat_devno, 0, "myplat_driver");
	} else {
		alloc_chrdev_region(&myplat_devno, 0, 1, "myplat_driver");
		myplat_major = MAJOR(myplat_devno);
	}
	return ret;
}

// 添加设备
static int myplat_driver_add(void)
{
	int ret;
	printk(KERN_NOTICE "myplat_driver_add\n");
	myplat_cdev = cdev_alloc();
	cdev_init(myplat_cdev, &myplat_ops);
	ret = cdev_add(myplat_cdev, myplat_devno, 1);
	return ret;
}

// 创建设备节点
static int myplat_driver_create(void)
{
	printk(KERN_NOTICE "myplat_driver_create\n");
	cls = class_create(THIS_MODULE, "myplat_driver");
	if (cls < 0) {
		printk(KERN_NOTICE "class_create fail\n");
		goto class_create_fail;
	}
	myplat_device = device_create(cls, NULL, myplat_devno, NULL, "myplat_device");
	if (myplat_device < 0) {
		printk(KERN_NOTICE "device_create fail\n");
		goto device_create_fail;
	}
	return 0;
class_create_fail:
	cdev_del(myplat_cdev);
	unregister_chrdev_region(myplat_devno, 1);
	return -EBUSY;
device_create_fail:
	class_destroy(cls);
	return -EBUSY;
}

static int myplat_driver_probe(struct platform_device *device)
{
	int ret;
	printk(KERN_NOTICE "myplat_driver_init\n");
	ret = myplat_driver_register();
	ret |= myplat_driver_add();
	ret |= myplat_driver_create();

	return ret;
}

static int myplat_driver_remove(struct platform_device *device)
{
	device_destroy(cls, myplat_devno);
	class_destroy(cls);
	cdev_del(myplat_cdev);
	unregister_chrdev_region(myplat_devno, 1);

	return 0;
}

static struct platform_driver myplat_driver = {
	.probe = myplat_driver_probe,
	.remove = myplat_driver_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "myplat",
	},
};

static int __init myplat_driver_init(void)
{
	int ret;
	ret = platform_driver_register(&myplat_driver);
	return ret;
}

static void __exit myplat_driver_exit(void)
{
	platform_driver_unregister(&myplat_driver);
}



module_init(myplat_driver_init);
module_exit(myplat_driver_exit);
MODULE_AUTHOR("Daniel <wannance@163.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("myplat_driver");


