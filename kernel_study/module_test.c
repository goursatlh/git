#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
/*
 * test lock in kernel:
 * 1, mutex
 * 2, spin lock
 * 3, rcu
 * */

struct task_struct *pthread1;
struct task_struct *pthread2;
int myglobal = 0;

/* mutex */
static DEFINE_MUTEX(my_mutex);

/* spin lock */
spinlock_t my_spinlock;
//spin_lock_init(my_spinlock);

#define MAX_LOOP 10000
static int thread_func(void *data)
{
    printk(KERN_ALERT "thread start: %d\n", pthread1->pid);
    //while (1)
    //while (!kthread_should_stop()) // kthread_should_stop should be added here, otherwise the thread will be hung up.
    {
        //printk(KERN_ALERT "hello world\n");
        int i, j;

        for (i = 0; i < MAX_LOOP; i++)
        {
            //mutex_lock(&my_mutex);
            //spin_lock(&my_spinlock);
            j = myglobal;
            j = j + 1;
            printk(".");
            //msleep(100);
            myglobal = j;
            //mutex_unlock(&my_mutex);
            //spin_unlock(&my_spinlock);
        }
        while (!kthread_should_stop())
        {
            printk(KERN_ALERT "thread: wait to stop......\n");
            msleep(1000);
        }
    }
    return 0;
}

static int __init hello_init(void)
{
    int i;
    spin_lock_init(&my_spinlock);
    printk(KERN_ALERT "Hello Linux kernel\n");
    pthread1 = kthread_run(thread_func, NULL, "hello");
    for (i = 0; i < MAX_LOOP; i++)
    {
        //mutex_lock(&my_mutex);
        //spin_lock(&my_spinlock);
        myglobal = myglobal + 1;
        printk("O");
        //mutex_unlock(&my_mutex);
        //spin_unlock(&my_spinlock);
        //msleep(100);
    }
    printk(KERN_ALERT "main: count finish\n");
    return 0;
}

static void __exit hello_exit(void)
{
    if (pthread1)
        kthread_stop(pthread1);
    printk(KERN_ALERT "main: last vaule %d\n", myglobal);
    printk(KERN_ALERT "Goodbye Linux kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);
