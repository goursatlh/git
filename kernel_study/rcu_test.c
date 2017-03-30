#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

struct task_struct  *pthread_reader;
struct task_struct  *pthread_updater;

typedef struct item
{
    int a;
    int b;
}iterm_t;

iterm_t *gp_ptr = NULL;

void func_reader(void *data)
{
    while (!kthread_should_stop())
    {
        if (gp_ptr)
            printk(KERN_ALERT "reader: a %x, b %x\n", gp_ptr->a, gp_ptr->b);
        else
            printk(KERN_ALERT "reader: gp_ptr is NULL\n");

        msleep(500);
    }
}

void func_updater(void *data)
{
    int i = 3, j = 4;
    while (!kthread_should_stop())
    {
        if (gp_ptr != NULL)
        {
            kfree(gp_ptr);
            gp_ptr = NULL;
        }
        gp_ptr = kmalloc(sizeof(iterm_t), GFP_KERNEL);
        gp_ptr->a = ++i;
        gp_ptr->b = ++j;
        printk(KERN_ALERT "updater: a %x, b %x\n", gp_ptr->a, gp_ptr->b);
        
        msleep(1000);
    }
}

void __init rcu_init(void)
{
    printk(KERN_ALERT "Module rcu test init.\n");
    gp_ptr = kmalloc(sizeof(iterm_t), GFP_KERNEL);
    gp_ptr->a = 1;
    gp_ptr->b = 2;
    pthread_reader = kthread_run(func_reader, NULL, "reader");
    pthread_updater = kthread_run(func_updater, NULL, "updater");
}

void __exit rcu_exit(void)
{
    kthread_stop(pthread_reader);
    kthread_stop(pthread_updater);
    printk(KERN_ALERT "Module rcu test exit.\n");
}

module_init(rcu_init);
module_exit(rcu_exit);
