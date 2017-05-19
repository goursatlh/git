#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
#include <linux/init.h>

#define INT2STR(a)  #a
char thread_name[128] = {0};
//spinlock_t my_spinlock;
rwlock_t rwlock;
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
        //spin_lock(&my_spinlock);
        read_lock(&rwlock);
        //rcu_read_lock();
        if (gp_ptr)
        {
            //msleep(1);
            printk(KERN_ALERT "reader: a: %d, b: %d\n", gp_ptr->a, gp_ptr->b);
        }
        else
            printk(KERN_ALERT "reader: gp_ptr is NULL\n");

        read_unlock(&rwlock);
        //rcu_read_unlock();
        //spin_unlock(&my_spinlock);
        msleep(1);
    }
}

void func_updater(void *data)
{
    int i = 3;
    iterm_t *ptmp = NULL;
    while (!kthread_should_stop())
    {
        //spin_lock(&my_spinlock);
        write_lock(&rwlock);
        //synchronize_rcu();
        if (gp_ptr != NULL)
        {
            kfree(gp_ptr);
        }
        gp_ptr = NULL;
        schedule();
        //msleep(1);
        ptmp = kmalloc(sizeof(iterm_t), GFP_KERNEL);
        ptmp->a = ++i;
        ptmp->b = ++i;
        gp_ptr = ptmp;
        //rcu_assign_pointer(gp_ptr, ptmp);
        write_unlock(&rwlock);
        //printk(KERN_ALERT "updater: a: %d, b: %d\n", gp_ptr->a, gp_ptr->b);
        //spin_unlock(&my_spinlock);
        msleep(1);
    }
}

char *gen_thread_name(int i, int flag)
{
    memset(thread_name, 0, sizeof(thread_name));
    if (flag)
        sprintf(thread_name, "reader%d", i);
    else
        sprintf(thread_name, "updater%d", i);
    return thread_name;
}

void __init rcu_init(void)
{
    int threadnum = 6, i = 0;
    printk(KERN_ALERT "Module rcu test init.\n");
    rwlock_init(&rwlock);
    //spin_lock_init(&my_spinlock);
    gp_ptr = kmalloc(sizeof(iterm_t), GFP_KERNEL);
    gp_ptr->a = 1;
    gp_ptr->b = 2;
    for (i = 0; i < threadnum; i++)
    {
        pthread_reader = kthread_run(func_reader, NULL, gen_thread_name(i, 1));
    }
    //pthread_reader->static_prio = 100;
    //printk(KERN_ALERT "reader static prio: %d, prio %d, normal prio %d.\n", 
           //pthread_reader->static_prio, pthread_reader->prio, pthread_reader->normal_prio);
    pthread_updater = kthread_run(func_updater, NULL, "updater");
    //printk(KERN_ALERT "updater static prio: %d, prio %d, normal prio %d.\n", 
           //pthread_updater->static_prio, pthread_updater->prio, pthread_updater->normal_prio);
    #if 0
    for (i = 0; i < threadnum; i++)
    {
        pthread_updater = kthread_run(func_updater, NULL, gen_thread_name(i, 0));
    }
#endif
}

void __exit rcu_exit(void)
{
    if (pthread_reader)
        kthread_stop(pthread_reader);
    if (pthread_updater)
        kthread_stop(pthread_updater);
    printk(KERN_ALERT "Module rcu test exit.\n");
}

module_init(rcu_init);
module_exit(rcu_exit);

MODULE_AUTHOR("Walter Wang");
MODULE_DESCRIPTION("Test locks in Linux Kernel");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:GHES");
