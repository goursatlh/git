//#include <linux/init.h>
//#include <linux/module.h>
//#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h> // msleep()
/*
 * test atomic operations in kernel:
 * */
struct task_struct *pthread1;
atomic_t myglobal = ATOMIC_INIT(0);
//int myglobal = 0; 
#define MAX_LOOP 1000000
static int thread_func(void *data)
{
    int i;

    printk(KERN_ALERT "thread start: %d\n", pthread1->pid);
    for (i = 0; i < MAX_LOOP; i++)
    {
        atomic_add(1, &myglobal);
        //++myglobal;
        printk(".");
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
    printk(KERN_ALERT "Hello Linux kernel pid %d %s\n", current->pid, current->comm);
    pthread1 = kthread_run(thread_func, NULL, "hello");
    for (i = 0; i < MAX_LOOP; i++)
    {
        atomic_add(1, &myglobal);
        //++myglobal;
        printk("O");
    }
    printk("\n");
    printk(KERN_ALERT "main: count finish, pid %d\n", current->pid);
    return 0;
}

static void __exit hello_exit(void)
{
    if (pthread1)
        kthread_stop(pthread1);
    printk(KERN_ALERT "main: last vaule %d\n", myglobal);
    printk(KERN_ALERT "Goodbye Linux kernel pid %d %s\n", current->pid, current->comm);
}

module_init(hello_init);
module_exit(hello_exit);
