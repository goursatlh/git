// test.c
// make -C /lib/modules/`uname -r`/build SUBDIRS=`pwd` modules
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>

static int pid =1;
module_param(pid,int,0644);
static unsigned long vaddr =0;
module_param(vaddr,long,0644);

// 根据一个进程的虚拟地址找到它的页表，相当于找到这家人的房子地址，然后闯入！
static pte_t* get_pte(struct task_struct *task, unsigned long address)
{
	pgd_t* pgd;
	p4d_t* p4d;
	pud_t* pud;
	pmd_t* pmd;
	pte_t* pte;
	struct mm_struct *mm = task->mm;
	pgd = pgd_offset(mm, address);
	if (pgd_none(*pgd)|| pgd_bad(*pgd))
		return NULL;
	p4d = p4d_offset(pgd, address);
	if (p4d_none(*p4d)|| p4d_bad(*p4d))
		return NULL;
	pud = pud_offset(p4d, address);
	if (pud_none(*pud)|| pud_bad(*pud))
		return NULL;
	pmd = pmd_offset(pud, address);
	if (pmd_none(*pmd)|| pmd_bad(*pmd))
		return NULL;
	pte = pte_offset_kernel(pmd, address);
	if (pte_none(*pte))
		return NULL;
    printk(KERN_ALERT "KERN_INFO vir addr 0x%lx, page table: %p %p %p %p %p\n", address, pgd, p4d, pud, pmd, pte);
	return pte;
}

static int test_init(void)
{
	struct task_struct *task;
	pte_t* pte;
	struct page* page;
    unsigned long vaddr1 =0;
    unsigned long paddr =0;
    unsigned long pfn = 0;
	// 找到这家人
	task = pid_task(find_pid_ns(pid,&init_pid_ns), PIDTYPE_PID);
	// 找到这家人住在哪里
	if(!(pte = get_pte(task, vaddr)))
		return -1;
	page = pte_page(*pte);
	// 强行闯入
	vaddr1 = page_address(page);
    paddr = page_to_phys(page);
    pfn = page_to_pfn(page);
    printk(KERN_ALERT "KERN_INFO pid %d, vaddr 0x%p --> paddr 0x%lx, pfn %d\n", pid, vaddr1, paddr, pfn);
	//strcpy(addr,(char*)"rain flooding water will not get fat!");
	return 0;
}

static void test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
