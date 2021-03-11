#include <linux/module.h>
#include <linux/slab.h>
#include <asm-generic/delay.h>
#include <linux/delay.h>

static int stop = 1;

// timer的数量
static int size = 1;
module_param(size, int, 0644);
MODULE_PARM_DESC(size, "size");

// timer的expires
static int interval = 200;
module_param(interval, int, 0644);
MODULE_PARM_DESC(interval, "");

#if 0
#define container_of(ptr, type, member) \
        (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#endif
// 回调函数耗时
static int dt = 100000;
module_param(dt, int, 0644);
MODULE_PARM_DESC(dt, "");

struct wrapper {
	struct timer_list timer;
    int data;
	spinlock_t lock;
};

struct wrapper *wr;

static void timer_func(struct timer_list *tw)
{
	struct wrapper *w = container_of(tw, struct wrapper, timer);

    printk(KERN_ALERT "timer(%d) handler(%p): jiffies %lu, timer size %d, dt %d , expires %d, stop %d\n", 
           w->data, w->timer, jiffies, size, dt, interval, stop);
	spin_lock_bh(&(w->lock));
	if (stop == 0) {
		mdelay(dt); // 以忙等模拟耗时
	}
	spin_unlock_bh(&(w->lock));

	if (stop == 0) {
		mod_timer(&(w->timer), jiffies + interval);
	}
}

static int __init maint_init(void)
{
	int i;

	wr = (struct wrapper *)kzalloc(size*sizeof(struct wrapper), GFP_KERNEL);

    printk(KERN_ALERT "soft lockup begin: %d\n", size);
	for (i = 0; i < size; i++) {
		struct wrapper *w = &wr[i];
		spin_lock_init(&(w->lock));
		w->timer.expires = jiffies + 20;
		w->timer.function = timer_func;
		w->data = i;
		add_timer(&(w->timer));
	}
	stop = 0;

	return 0;
}

static void __exit maint_exit(void)
{
	int i;

    printk(KERN_ALERT "soft lockup end \n");
	stop = 1;
	udelay(100);
	for (i = 0; i < size; i++) {
		struct wrapper *w = &wr[i];
		//del_timer_sync(&(w->timer));
		del_timer(&(w->timer));
	}
	kfree(wr);

}

module_init(maint_init);
module_exit(maint_exit);
MODULE_LICENSE("GPL");

