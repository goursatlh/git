#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/slab.h>

/*
 * test lock in kernel:
 * 1, mutex
 * 2, spin lock
 * 3, rcu
 * */

struct task_struct *pthread1;
int myglobal = 0;

/* mutex */
//static DEFINE_MUTEX(my_mutex);

/* spin lock */
spinlock_t my_spinlock;
//spin_lock_init(my_spinlock);

#define MAX_LOOP 10000000
static int thread_func(void *data)
{
    int i, j;

    printk(KERN_ALERT "thread start: %d\n", pthread1->pid);
    msleep(300);
    //for (i = 0; i < MAX_LOOP; i++)
    while (1)
    {
        printk(KERN_ALERT "thread(%d) want to acquire the spinlock\n", pthread1->pid);
        //mutex_lock(&my_mutex);
        spin_lock(&my_spinlock);
        printk(KERN_ALERT "thread(%d) hold the spinlock\n", pthread1->pid);
        j = myglobal;
        j = j + 1;
        j = j - 1;
        //printk(".");
        myglobal = j;
        //mutex_unlock(&my_mutex);
        spin_unlock(&my_spinlock);
        msleep(1);
        //printk(KERN_ALERT "thread(%d) releases the spinlock\n", pthread1->pid);
    }
    printk("\n");
    printk(KERN_ALERT "thread finish: %d\n", pthread1->pid);
    while (!kthread_should_stop())
    {
        printk(KERN_ALERT "thread: wait to stop......\n");
        msleep(1000);
    }
    return 0;
}

static int __init hello_init(void)
{
    int i;
    spin_lock_init(&my_spinlock);
    printk(KERN_ALERT "main Hello Linux kernel pid %d %s\n", current->pid, current->comm);
    //pthread1 = kthread_run(thread_func, NULL, "hello");
    printk(KERN_ALERT "main thread(%d) want to acquire the spinlock\n", current->pid);
    spin_lock(&my_spinlock);
    printk(KERN_ALERT "main thread(%d) hold the spinlock\n", current->pid);
    printk(KERN_ALERT "main thread(%d) sleeping...\n", current->pid);
    msleep(10000000);
    //for (i = 0; i < MAX_LOOP; i++)
#if 0
    while (1)
    {
        //mutex_lock(&my_mutex);
        spin_lock(&my_spinlock);
        //msleep(5000);
        myglobal = myglobal + 1;
        myglobal = myglobal - 1;
        //printk("O");
        //mutex_unlock(&my_mutex);
        spin_unlock(&my_spinlock);
        msleep(1);
    }
#endif
    //msleep(100000);
    //spin_unlock(&my_spinlock);
    printk(KERN_ALERT "main thread(%d) release the spinlock\n", current->pid);
    printk("\n");
    printk(KERN_ALERT "main count finish, pid %d\n", current->pid);
    return 0;
}

static void __exit hello_exit(void)
{
    if (pthread1)
        kthread_stop(pthread1);
    printk(KERN_ALERT "main: last value %d\n", myglobal);
    printk(KERN_ALERT "Goodbye Linux kernel pid %d %s\n", current->pid, current->comm);
}

module_init(hello_init);
module_exit(hello_exit);
