#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource myplat_resource[] = {
    [0] = {
    	.start = 0x10000000,
    	.end = 0x10000000 + 4,
    	.flags = IORESOURCE_MEM,
     },
};

static struct platform_device myplat_device = {
	.name = "myplat",
	.num_resources = ARRAY_SIZE(myplat_resource),
	.resource = myplat_resource,
};

static int __init myplat_device_init(void)
{
	printk(KERN_NOTICE "myplat_device_init\n");
	platform_device_register(&myplat_device);
	return 0;
}

static void __exit myplat_device_exit(void)
{
	printk(KERN_NOTICE "myplat_device_exit\n");
	platform_device_unregister(&myplat_device);
}

module_init(myplat_device_init);
module_exit(myplat_device_exit);
MODULE_AUTHOR("Daniel <wannance@163.com>");
MODULE_LICENSE("GPL");
