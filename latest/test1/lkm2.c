#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("Printing Child Processes");

static int target_pid = 0;  
module_param(target_pid, int, S_IRUGO);  

// init function that takes a pid as argument and prints child processes
static int __init lkm_init(void) {
    
    struct task_struct *parent_task, *child_task;
    printk("Listing child processes of PID %d:\n", target_pid);

    // Find the parent process with the given PID
    parent_task = pid_task(find_get_pid(target_pid), PIDTYPE_PID);

    if (parent_task == NULL) {
        printk(KERN_ERR "Process with PID %d not found.\n", target_pid);
        return -EINVAL;  
    }

    // Iterate through child processes
    list_for_each_entry(child_task, &parent_task->children, sibling) {
        printk("Child PID: %d, State: %ld\n", child_task->pid, child_task->__state);
    }

    return 0;  
}

// exit function
static void __exit lkm_exit(void) {
    printk(KERN_INFO "Kernel module unloaded.\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
