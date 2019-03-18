#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/slab.h>

static int __init hello_init(void)
{
    int *p = kmalloc(128, GFP_KERNEL);
    if (p == NULL)
    {
        printk("kmalloc failed.\n");
        return -1;
    }
    p += 2560000;
    *p = 1;
    printk(KERN_ALERT "Hello Linux kernel\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Goodbye Linux kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);
