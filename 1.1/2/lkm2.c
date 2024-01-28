#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("Kernel Module to Print Child Processes");

static int target_pid = 0;  // Default value, should be set when loading the module

module_param(target_pid, int, S_IRUGO);  // Define module parameter for target PID

// Module initialization function
static int __init lkm_init(void) {
    struct task_struct *parent_task, *child_task;

    printk(KERN_INFO "Listing child processes of PID %d:\n", target_pid);

    // Find the parent process with the given PID
    parent_task = pid_task(find_get_pid(target_pid), PIDTYPE_PID);

    if (parent_task == NULL) {
        printk(KERN_ERR "Process with PID %d not found.\n", target_pid);
        return -EINVAL;  // Return an error code
    }

    // Iterate through child processes
    list_for_each_entry(child_task, &parent_task->children, sibling) {
        printk(KERN_INFO "Child PID: %d, State: %ld\n", child_task->pid, child_task->__state);
    }

    return 0;  // Initialization successful
}

// Module cleanup function
static void __exit lkm_exit(void) {
    printk(KERN_INFO "Kernel module unloaded.\n");
}

// Register module initialization and cleanup functions
module_init(lkm_init);
module_exit(lkm_exit);
