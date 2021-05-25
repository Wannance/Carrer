#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource hello_resource[] = {
	[0] = {
		.start = 0x10000000,
		.end = 0x10000000 + 4,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device hello_device = {
	.name = "hello_platform",
	.num_resources = ARRAY_SIZE(hello_resource),
	.resource = hello_resource,
};

static int __init hello_device_init(void)
{
	int ret;
	printk(KERN_INFO "hello init\n");
	ret = platform_device_register(&hello_device);

	return ret;
}

static void __exit hello_device_exit(void)
{
	printk(KERN_INFO "hello exit\n");
	platform_device_unregister(&hello_device);	
}

module_init(hello_device_init);
module_exit(hello_device_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Daniel <wannance@163.com>");
MODULE_ALIAS("hello_device");
