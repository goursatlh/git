#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/slab.h>

//memory alloc
#include <linux/gfp.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
/*
 * test memory allocation in kernel space:
 * 1, alloc_pages()
 * 2, kmem_cache_alloc
 * 3, kmalloc
 * 4, vmalloc
 * */

static int __init memx_init(void)
{
    struct page *pg = NULL;
    struct kmem_cache *mem_cache = NULL;
    unsigned long paddr = 0;
    unsigned long cout = 1;
    unsigned long num = 0;
    unsigned long pfn = 0;
    void *pmem = NULL;
    printk(KERN_ALERT "mem test init\n");

	mem_cache = kmem_cache_create(   "mem_test",
									 4096,
									 0,
									 SLAB_HWCACHE_ALIGN, 
									 NULL
								 );
    while (cout--)
    {
#if 1 // alloc_pages
        pg = alloc_pages(GFP_KERNEL, 0);
        if (pg > 0)
        {
            printk(KERN_ALERT "alloc_pages return %p\n", pg);
            paddr = page_to_phys(pg);
            printk(KERN_ALERT "pages to phy addr %lx\n", paddr);
            pfn = page_to_pfn(pg);
            printk(KERN_ALERT "pages to pfn %lx\n", pfn);
        }
        else
        {
            printk(KERN_ALERT "alloc_pages return error %p\n", pg);
            return 0;
        }
#endif
        ++num;
#if 0 
		pmem = kmem_cache_alloc(mem_cache, GFP_ATOMIC);
        if (pmem > 0)
            printk(KERN_ALERT "kmem_cache_alloc %lu return %p\n", num, pmem);
        else
        {
            printk(KERN_ALERT "kmem_cache_alloc return error %p\n", pmem);
            return 0;
        }
#endif
    }
    return 0;
}

static void __exit memx_exit(void)
{
    printk(KERN_ALERT "mem test exit\n");
}

module_init(memx_init);
module_exit(memx_exit);

MODULE_LICENSE("GPL v2");
