#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <scsi/scsi_transport_iscsi.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DhruvK");
MODULE_DESCRIPTION("List Running Processes");

// init function that iterates all processes and prints running processes
static int __init lkm_init(void) {
    
    struct task_struct *task;
    printk("Listing all running processes:\n");

    // Iterate through all processes
    for_each_process(task) {

        // Check if the process is in a running or runnable state
        if (task->__state == TASK_RUNNING || task->__state == TASK_INTERRUPTIBLE || task->__state == TASK_UNINTERRUPTIBLE) {
            printk("PID: %d, Name: %s\n", task->pid, task->comm);
        }
    }
    return 0;  
}

// exit function
static void __exit lkm_exit(void) {
    printk(KERN_INFO "Kernel module unloaded.\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
