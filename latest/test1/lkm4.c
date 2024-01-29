#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/pid.h>

#include <linux/pgtable.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <asm/page.h>
#include <linux/rmap.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("Printing allocated and mapped memory");

static int pid = 0;
module_param(pid, int, S_IRUGO);

// calculating allocated and mapped address space for a given pid
static void __init calc_address_space_size(pid_t pid)
{
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    unsigned long vsize, psize;
    struct page_vma_mapped_walk pvmw;

    // Find the task_struct of the process by pid
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk("No such process with pid %d\n", pid);
        return;
    }

    // Get the mm_struct of the process
    mm = task->mm;
    if (!mm) {
        printk("No mm_struct for process %s\n", task->comm);
        return;
    }
   
    // Initialize the size variables
    vsize = 0;
    psize = 0;
    unsigned long res,i,flag;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;

    // Iterate over the vma list of the mm_struct
    VMA_ITERATOR(iter, mm, 0);

    for_each_vma(iter, vma) {
        
        // Calculate the size of the virtual address space for each vma
        vsize += vma->vm_end - vma->vm_start;
        for(i=vma->vm_start;i!=vma->vm_end;i+=PAGE_SIZE){
            
            flag = 1;
            pgd = pgd_offset(mm, i);
            
            if (!pgd_present(*pgd)) {
                // Top-level page table entry is not present.
                flag = 0;
            }

            p4d = p4d_offset(pgd,i);
                // 4th level page table entry is not present.
            if(!p4d_present(*p4d)){
                flag = 0;
            }

            pud = pud_offset(p4d, i);
            if (!pud_present(*pud)) {
                // Second-level page table entry is not present.
                flag = 0;
            }

            pmd = pmd_offset(pud, i);
            if (!pmd_present(*pmd)) {
                // Third-level page table entry is not present.
                flag = 0;
            }

            pte = pte_offset_map(pmd, i);
            if (!pte_present(*pte)) {
                // Page table entry is not present.
                flag = 0;
            }

        
            if((res = virt_to_phys(i)) && flag!=0){
                psize += PAGE_SIZE;
            }
        }
        
    }

    // Print the results
    printk(KERN_INFO "Process %s (pid %d) has allocated %lu Kbytes of virtual address space\n", task->comm, pid, vsize/(1024));
    printk(KERN_INFO "Process %s (pid %d) has mapped %ld Kbytes of physical address space\n", task->comm, pid, psize/(1024));
}

// init function
static int __init my_module_init(void)
{
    // Test the function with some pid
    calc_address_space_size(pid);
    return 0;
}

// exit function
static void __exit my_module_exit(void)
{
    printk("Module unloaded\n");
}


module_init(my_module_init);
module_exit(my_module_exit);


