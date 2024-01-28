#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("Kernel Module to Check Virtual-to-Physical Mapping");

static int target_pid = 0;
static unsigned long target_virtual_address = 0;

module_param(target_pid, int, S_IRUGO);
module_param(target_virtual_address, ulong, S_IRUGO);

// Module initialization function
static int __init lkm_init(void) {
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    phys_addr_t phys_addr = 0;

    printk(KERN_INFO "Checking virtual-to-physical mapping for PID %d, Virtual Address %lu:\n", target_pid, target_virtual_address);

    // Find the task_struct for the given PID
    task = pid_task(find_get_pid(target_pid), PIDTYPE_PID);

    if (task == NULL) {
        printk(KERN_ERR "Process with PID %d not found.\n", target_pid);
        return -EINVAL;  // Return an error code
    }

    // Access the task's memory descriptor
    mm = get_task_mm(task);

    if (mm == NULL) {
        printk(KERN_ERR "Could not access memory descriptor for PID %d.\n", target_pid);
        return -EINVAL;  // Return an error code
    }

    // Iterate through the VMAs (Virtual Memory Areas)
    down_read(&mm->mmap_lock);
    vma = find_vma(mm, target_virtual_address);

    if (vma != NULL && target_virtual_address >= vma->vm_start && target_virtual_address < vma->vm_end) {
        // The virtual address is within a valid VMA
        phys_addr = virt_to_phys((void *)target_virtual_address);
        printk(KERN_INFO "PID: %d, Virtual Address: %lu, Physical Address: %llx\n", target_pid, target_virtual_address, (unsigned long long)phys_addr);
    } else {
        printk(KERN_INFO "Virtual address %lu is not mapped in the address space of PID %d.\n", target_virtual_address, target_pid);
    }

    up_read(&mm->mmap_lock);
    mmput(mm);

    return 0;  // Initialization successful
}

// Module cleanup function
static void __exit lkm_exit(void) {
    printk(KERN_INFO "Kernel module unloaded.\n");
}

// Register module initialization and cleanup functions
module_init(lkm_init);
module_exit(lkm_exit);
