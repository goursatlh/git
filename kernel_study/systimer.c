#include <linux/init.h>
#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/delay.h>     
#include <linux/spinlock.h>     

spinlock_t lock;
unsigned long flags;

static int __init systimer_init(void)
{
    struct timeval now;
    spin_lock_init(&lock);
    printk( KERN_DEBUG "sys timer begin\n" );
    int count = 100;
    while (--count > 0)
    {
        do_gettimeofday(&now);
        printk(KERN_ALERT "start time: %lu: %lu\n", now.tv_sec, now.tv_usec);
        spin_lock_irqsave(&lock, flags); 
        printk( KERN_DEBUG "the jiffies is %ld\n" ,jiffies);
        mdelay(2000);
        printk( KERN_DEBUG "the jiffies is %ld\n" ,jiffies);
        do_gettimeofday(&now);
        printk(KERN_ALERT "mid time: %lu: %lu\n", now.tv_sec, now.tv_usec);
        spin_unlock_irqrestore(&lock, flags);
        do_gettimeofday(&now);
        printk(KERN_ALERT "end time: %lu: %lu\n", now.tv_sec, now.tv_usec);
        msleep(2000);
    }
    return 0;
}
    
static void __exit systimer_exit(void)
{
    printk(KERN_ALERT "sys timer test end\n");
}

module_init(systimer_init);
module_exit(systimer_exit);

MODULE_AUTHOR("Walter Wang");
MODULE_DESCRIPTION("slowdown the sys timer module");
MODULE_LICENSE("GPL");
