#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
//#include //for_each_process

//module_param(pid, int, S_IRUGO);

static int killd_init(void)
{
    struct task_struct *p;
    struct task_struct *g, *t;
    printk(KERN_ALERT "threadswalk: start\n");

    //read_lock(&tasklist_lock);
    //for_each_process(p)
    do_each_thread(g,t)
    {
        printk(KERN_ALERT "g %p, t %p\n", g, t);
        printk(KERN_ALERT "g pid %u, %u, %s thread num: %d, group lead: %p\n", 
               g->pid, g->tgid, g->comm, g->signal->nr_threads, g->group_leader);
        printk(KERN_ALERT "t pid %u, %u, %s thread num: %d, gourp lead: %p\n", 
               t->pid, t->tgid, t->comm, t->signal->nr_threads, t->group_leader);
    }while_each_thread (g, t);
    //read_unlock(&tasklist_lock);
    return 0;
}

static void killd_exit(void)
{
    printk(KERN_ALERT "threadswalk: bye\n");
}

module_init(killd_init);
module_exit(killd_exit);

