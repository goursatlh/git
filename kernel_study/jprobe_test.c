#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/skbuff.h>
#include <net/sock.h>


int jip_local_out(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	pr_info("jprobe: sk = 0x%lx, skb = 0x%lx ", sk, skb);
	/* Always end with a call to jprobe_return(). */
	jprobe_return();
	return 0;
}

static struct jprobe my_jprobe = {
	.entry			= jip_local_out,
	.kp = {
		.symbol_name	= "ip_local_out",
	},
};

static int __init jprobe_init(void)
{
	int ret;
 
	ret = register_jprobe(&my_jprobe);
	if (ret < 0) {
		printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n",
	       my_jprobe.kp.addr, my_jprobe.entry);
	return 0;
}
 
static void __exit jprobe_exit(void)
{
	unregister_jprobe(&my_jprobe);
	printk(KERN_INFO "jprobe at %p unregistered\n", my_jprobe.kp.addr);
}
 
module_init(jprobe_init)
module_exit(jprobe_exit)
MODULE_LICENSE("GPL");
