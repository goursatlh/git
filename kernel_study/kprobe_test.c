#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/kprobes.h>


/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	//.symbol_name	= "_do_fork",
	//.symbol_name	= "__do_sys_fork",
	.symbol_name	= "__do_sys_clone",
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
#ifdef CONFIG_X86
	printk(KERN_INFO "pre_handler: p->addr = 0x%lx, ip = %lx,"
			" flags = 0x%lx\n",
		p->addr, regs->ip, regs->flags);
#endif
#ifdef CONFIG_PPC
	printk(KERN_INFO "pre_handler: p->addr = 0x%p, nip = 0x%lx,"
			" msr = 0x%lx\n",
		p->addr, regs->nip, regs->msr);
#endif
#ifdef CONFIG_MIPS
	printk(KERN_INFO "pre_handler: p->addr = 0x%p, epc = 0x%lx,"
			" status = 0x%lx\n",
		p->addr, regs->cp0_epc, regs->cp0_status);
#endif
#ifdef CONFIG_TILEGX
	printk(KERN_INFO "pre_handler: p->addr = 0x%p, pc = 0x%lx,"
			" ex1 = 0x%lx\n",
		p->addr, regs->pc, regs->ex1);
#endif
 
	/* A dump_stack() here will give a stack backtrace */
	return 0;
}


/* kprobe post_handler: called after the probed instruction is executed */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
#ifdef CONFIG_X86
	printk(KERN_INFO "post_handler: p->addr = 0x%lx, flags = 0x%lx\n",
		p->addr, regs->flags);
#endif
#ifdef CONFIG_PPC
	printk(KERN_INFO "post_handler: p->addr = 0x%p, msr = 0x%lx\n",
		p->addr, regs->msr);
#endif
#ifdef CONFIG_MIPS
	printk(KERN_INFO "post_handler: p->addr = 0x%p, status = 0x%lx\n",
		p->addr, regs->cp0_status);
#endif
#ifdef CONFIG_TILEGX
	printk(KERN_INFO "post_handler: p->addr = 0x%p, ex1 = 0x%lx\n",
		p->addr, regs->ex1);
#endif
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk(KERN_INFO "fault_handler: p->addr = 0x%lx, trap #%dn",
		p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}
 
static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;
 
	ret = register_kprobe(&kp);
	if (ret < 0) {
		printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
	return 0;
}
 
static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}
 
module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
