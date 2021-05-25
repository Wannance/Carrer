#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#define HELLO_MAJOR 233
#define HELLO_MINOR 0
#define HELLO_MEM_SIZE 1024

static dev_t hello_dev;
static int hello_major;
static struct cdev *hello_cdev;
static struct class *cls = NULL;
static struct device *hello_device = NULL;
static struct file_operations hello_ops = {
	.owner = THIS_MODULE,
};

// 创建设备号
static int hello_register_chrdev(void)
{
	int ret;
	if (hello_major) {
		register_chrdev_region(hello_dev, 1, "hello_driver");
	} else {
		ret = alloc_chrdev_region(&hello_dev, 0, 1, "hello_driver");
		hello_major = MAJOR(hello_dev);
		if (ret < 0) {
			printk(KERN_INFO "alloc_chrdev_region\n");
			return ret;
		}
	}
	return 0;
}

// 注册驱动
static int hello_cdev_add(void)
{
	int ret;
	hello_cdev = cdev_alloc();
	cdev_init(hello_cdev, &hello_ops);
	ret = cdev_add(hello_cdev, hello_dev, 1);
	if (ret < 0) {
		unregister_chrdev_region(hello_dev, 1);
		printk(KERN_INFO "hello_driver_cdev_add fail\n");
		return ret;		
	}
	return 0;
}

// 创建设备节点
static int hello_device_create(void)
{
	cls = class_create(THIS_MODULE, "hello_driver");
	if (IS_ERR(cls)) {
		printk(KERN_INFO "hello class_create fail\n");
		goto class_create_fail;
	}
	hello_device = device_create(cls, NULL, hello_dev, NULL, "hello_dev");
	if (IS_ERR(hello_device)) {
		printk(KERN_INFO "hello device_create fail\n");
		goto device_create_fail;
	}
	return 0;

class_create_fail:
	cdev_del(hello_cdev);
	unregister_chrdev_region(hello_dev, 1);
	return -1;
device_create_fail:
	class_destroy(cls);
	return -1;
}

static int hello_probe(struct platform_device *hello_device)
{
	int ret;
	ret = hello_register_chrdev();
	if (ret < 0) {
		return ret;
	}
	ret = hello_cdev_add();
	if (ret < 0) {
		return ret;
	}
	ret = hello_device_create();
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static int hello_remove(struct platform_device *hello_device)
{
	printk(KERN_INFO "hello remove\n");
	device_destroy(cls, hello_dev);
	class_destroy(cls);
	cdev_del(hello_cdev);
	unregister_chrdev_region(hello_dev, 1);

	return 0;
}

static struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.driver = {
		.name = "hello_platform",
		.owner = THIS_MODULE,
	},
};

static int __init hello_driver_init(void)
{
	return(platform_driver_register(&hello_driver));
}

static void __exit hello_driver_exit(void)
{
	platform_driver_unregister(&hello_driver);
}

module_init(hello_driver_init);
module_exit(hello_driver_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Daniel <wannance@163.com>");
MODULE_ALIAS("hello_driver");

