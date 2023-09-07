#include <linux/init.h>
#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/cpu.h>     
#include <linux/percpu.h>     

#include <asm/local.h>

static unsigned long addr = 0;
module_param(addr, ulong, S_IRUGO);

//int force(void *data)
void force(void)
{
    //return 0;
    return;
}
    
static int __init rm_init(void)
{
    int i = 0, cpu;
    printk(KERN_ALERT "Hello Linux kernel, input para: %x\n", addr);
    //struct module *mod = (struct module *)0xffffffffc09f9040;
    struct module *mod = (struct module *)addr;
    printk(KERN_ALERT "force rm module %s, module state %d, reference %d\n", 
                       mod->name, mod->state, module_refcount(mod));
    mod->state = MODULE_STATE_LIVE;
    mod->exit = force;

#if 0
    for (i = 0; i < NR_CPUS; i++) {
        (mod->refptr[i])->incs = 0;  
        (mod->refptr[i])->decs = 0;  
    }
#endif
#if 0
    for_each_possible_cpu(cpu)
    {
        per_cpu_ptr(mod->refptr, cpu)->decs = 0;
        per_cpu_ptr(mod->refptr, cpu)->incs = 0;
    }
#endif
    //linux-5.15.0-71
    for_each_possible_cpu(cpu)
    {
        local_set((local_t*)per_cpu_ptr(&(mod->refcnt), cpu), 0);
    }
    printk(KERN_ALERT "force rm module %s, module state %d, reference %d %d\n", 
                       mod->name, mod->state, module_refcount(mod), atomic_read(&mod->refcnt));

    atomic_set(&mod->refcnt, 1);
    printk(KERN_ALERT "force rm module %s, module state %d, reference %d %d\n", 
                       mod->name, mod->state, module_refcount(mod), atomic_read(&mod->refcnt));
    return 0;
}
    
static void __exit rm_exit(void)
{
    printk(KERN_ALERT "Goodbye Linux kernel\n");
}

module_init(rm_init);
module_exit(rm_exit);

MODULE_AUTHOR("Walter Wang");
MODULE_DESCRIPTION("force rm kernel module");
MODULE_LICENSE("GPL");
