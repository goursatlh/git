#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>

#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

static struct hrtimer timer;
ktime_t kt;
struct timespec64 oldtc, tc;

char *buf, *meminfo, *timeinfo;
struct file *file = NULL, *outfile = NULL;
int read_cnt = 0;
loff_t wr_pos;

static void parse_meminfo(char* buf, char* info, int bufSize, int infoSize) {
    //some operations of the raw string
}


static enum hrtimer_restart hrtimer_hander(struct hrtimer *timer)
{
    //char* f = "/tmp/test.txt";
    //struct file* fs = filp_open(f, O_RDWR | O_APPEND | O_CREAT, 0644);
    //filp_close(fs, NULL); 

    printk(KERN_ALERT "hrtimer timeout.\n");
    return HRTIMER_RESTART;
}

#if 0
static enum hrtimer_restart hrtimer_hander(struct hrtimer *timer)
{
    ktime_get_real_ts64(&tc); 
    oldtc = tc;
    
    snprintf(timeinfo, 21, "Timestamp:%lld", tc.tv_sec);   
    read_cnt = kernel_read(file, buf, 140, 0);
    parse_meminfo(buf, meminfo, 140, 93);
        
    // meminfo[92] = 0;
    // printk("(debug)%s%s\n", timeinfo, meminfo);
    // The printk() works well    
    
    kernel_write(outfile, (const void *)meminfo, 93, &wr_pos);

    hrtimer_forward(timer,timer->base->get_time(),kt);
    return HRTIMER_RESTART;
}
#endif
 
static int __init fileirq_init(void)
{
    printk("(debug)---------test start-----------\r\n");
    
    ktime_get_real_ts64(&oldtc);
    kt = ktime_set(2, 0); 
    hrtimer_init(&timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
    hrtimer_start(&timer,kt,HRTIMER_MODE_REL);
    timer.function = hrtimer_hander;

#if 0
    buf = (char*)kmalloc(140, GFP_KERNEL);
    meminfo = (char*)kmalloc(93, GFP_KERNEL);
    timeinfo = (char*)kmalloc(21, GFP_KERNEL);
    wr_pos = 0;

    if(file == NULL) {
        file = filp_open("/proc/meminfo", O_RDONLY, 0);
    }
    if(IS_ERR(file)) {
        printk("(!!debug)error occured while opening /proc/meminfo \n");
        return 0;
    } 
    else {
        printk("(debug)succefffully open /proc/meminfo");
    }
    
    if(outfile == NULL) {
        outfile = filp_open("/var/log/time_mem_info", O_WRONLY | O_APPEND | O_CREAT, 0664);
    }
    if(IS_ERR(outfile) || outfile == NULL) {
        printk("(!!debug)error occured while opening /var/log/time_mem_info \n");
        return 0;
    } 
    else {
        printk("(debug)succefffully open /var/log/time_mem_info");
    }
#endif
    return 0;
}
    
static void __exit fileirq_exit(void)
{
    printk(KERN_ALERT "sys timer test end\n");
}

module_init(fileirq_init);
module_exit(fileirq_exit);

MODULE_AUTHOR("Walter Wang");
MODULE_DESCRIPTION("File-opeartions in irq context are very very wrong");
MODULE_LICENSE("GPL");
