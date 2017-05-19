#include <linux/init.h>
#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/cpu.h>     
#include <linux/percpu.h>     
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define MXP_MEM_BLK_ALLOC 1
#define MXP_MEM_DEVICE_NAME "timxpmem"
struct TEST_MSG {
    void *data;
};

//module_param(addr, ulong, S_IRUGO);
long mxpmem_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
    struct TEST_MSG msg;
    int res;
    if (__copy_from_user((void *)&msg, (void __user *) ioctl_param, (unsigned long) sizeof(struct TEST_MSG)))
    {
        res = -EFAULT;
        goto error_label;
    }

    switch (ioctl_num)
    {
        case MXP_MEM_BLK_ALLOC:     
        {
            schedule();
            msleep(1000);
            u8 *user_addr = (u8 *)(msg.data);
            printk(KERN_ALERT "user space addr %p, value %d\n", user_addr, *user_addr);
            *user_addr = 38;
            printk(KERN_ALERT "user space addr %p, value %d after change\n", user_addr, *user_addr);
            break;
        }
        default:
            break;
    }

error_label:
    return 0;
}

static struct file_operations mxpmem_fops = {
    unlocked_ioctl:     mxpmem_ioctl,
};

struct miscdevice mxpmem_miscdev = {
        minor: MISC_DYNAMIC_MINOR,
        name:  MXP_MEM_DEVICE_NAME,
        fops:  &mxpmem_fops,
};

static int __init rm_init(void)
{
    int err;
    printk(KERN_ALERT "Hello Linux kernel\n");
    
    if ((err = misc_register(&mxpmem_miscdev)))
    {
        printk(KERN_ERR "mxpmem: cannot register miscdev err=%d\n", err);
        return err;
    }  
    //u8 *user_addr = (u8 *)addr;
    //printk(KERN_ALERT "user space addr %p, value %d\n", user_addr, *user_addr);
    //*user_addr = 38;
    //printk(KERN_ALERT "user space addr %p, value %d after change\n", user_addr, *user_addr);
   
    //proc_create("hello_proc", 0, NULL, &hello_proc_fops);
    //char *p = kmalloc(100, GFP_KERNEL);
   
    return 0;
}
    
static void __exit rm_exit(void)
{    
    int err;
    if((err = misc_deregister(&mxpmem_miscdev)))
    {
        printk(KERN_ERR "mxpmem: cannot de-register miscdev err=%d\n", err);
    }
    //remove_proc_entry("hello_proc", NULL);
    printk(KERN_ALERT "Goodbye Linux kernel\n");
}

module_init(rm_init);
module_exit(rm_exit);

MODULE_AUTHOR("Walter Wang");
MODULE_DESCRIPTION("force rm kernel module");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("Texas Instruments "MXP_MEM_DEVICE_NAME);
