#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("Simple module featuring proc read");

#define ENTRY_NAME "hello_world"
#define PERMS 0644
#define PARENT NULL

static struct proc_ops fops;
static char *message;
static int read_p;

int hello_proc_open(struct inode *sp_inode, struct file *sp_file) {
    printk("proc called open\n");
    
    read_p = 1;
    message = kmalloc(sizeof(char) * 20, GFP_KERNEL |__GFP_IO | __GFP_FS);
    if (message == NULL) {
        printk("ERROR, hello_proc_open");
        return -ENOMEM;
    }
    strcpy(message, "Hello, World!\n");
    return 0;
}

ssize_t hello_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset) {

    int len = strlen(message);
    read_p = !read_p;
    if (read_p) {
        return 0;
    }

    printk("proc called read\n");
    copy_to_user(buf, message, len);
    return len;
}

int hello_proc_release(struct inode *sp_inode, struct file *sp_file) {
    
    printk("proc called release\n");
    kfree(message);
    return 0;
}

static int __init hello_init(void) {
    printk("/proc/%s create\n", ENTRY_NAME);
    fops.proc_open = hello_proc_open;
    fops.proc_read = hello_proc_read;
    fops.proc_release = hello_proc_release;

    if (!proc_create(ENTRY_NAME, PERMS, NULL, &fops)) {
    printk("ERROR! proc_create\n");
    remove_proc_entry(ENTRY_NAME, NULL);
    return -ENOMEM;
    }
    return 0;
}

static void __exit hello_exit(void) {
    remove_proc_entry(ENTRY_NAME, NULL);
    printk("Removing /proc/%s.\n", ENTRY_NAME);
}

module_init(hello_init);
module_exit(hello_exit);