#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include "io_driver_dr_doom.h"

#define MN_CNT 1
#define FS_MN 0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("driver for changing parent of a process");

static struct class *cl;
static struct cdev dv_c;
static dev_t dv_o;

static void change_parent(pid_t soldier_pid, pid_t control_station_pid)
{
    struct list_head *hd_ctrl_st, *hd_sl, *hd_tmp;
    struct task_struct *tsk_ctrl_st, *tsk_sl;

    tsk_sl = pid_task(find_get_pid(soldier_pid), PIDTYPE_PID);
    tsk_ctrl_st = pid_task(find_get_pid(control_station_pid), PIDTYPE_PID);

    if (tsk_ctrl_st == NULL || tsk_sl == NULL)
    {
        printk("ERR: Invalid pid given");
        return;
    }

    // find solider in soldier's parent's descendent list
    hd_sl = &tsk_sl->parent->children;

    list_for_each(hd_tmp, hd_sl)
    {
        if (list_entry(hd_tmp, struct task_struct, sibling)->pid == tsk_sl->pid)
        {
            printk("Found a soldier in parent's descendent list\n");
            printk("Deleted the soldier from parent's descendent list\n");
            list_del(hd_tmp);
            break;
        }
    }

    tsk_sl->parent = tsk_ctrl_st;
    tsk_sl->real_parent = tsk_ctrl_st;
    printk("Changed parent of Soldier\n");

    // Adding soldier to control station's children list
    hd_ctrl_st = &tsk_ctrl_st->children;
    list_add(&tsk_sl->sibling, hd_ctrl_st);

    printk("%d %d\n", soldier_pid, control_station_pid);
    return;
}

static int new_open(struct inode *i, struct file *f)
{
    return 0;
}
static int new_close(struct inode *i, struct file *f)
{
    return 0;
}
static long new_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    driver_arg_t darg;

    switch (cmd)
    {
    case CHANGE_PARENT:
        if (copy_from_user(&darg, (driver_arg_t *)arg, sizeof(driver_arg_t)))
        {
            return -EACCES;
        }
        change_parent(darg.self_pid, darg.parent_pid);
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations driver_fops =
    {
        .owner = THIS_MODULE,
        .open = new_open,
        .release = new_close,
        .unlocked_ioctl = new_ioctl};

static int __init ioctl_init(void)
{
    int ret;
    struct device *dev_ret;

    if ((ret = alloc_chrdev_region(&dv_o, FS_MN, MN_CNT, "ioctl_dr_doom")) < 0)
    {
        return ret;
    }

    cdev_init(&dv_c, &driver_fops);

    if ((ret = cdev_add(&dv_c, dv_o, MN_CNT)) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv")))
    {
        cdev_del(&dv_c);
        unregister_chrdev_region(dv_o, MN_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dv_o, NULL, "ioctl_dr_doom")))
    {
        class_destroy(cl);
        cdev_del(&dv_c);
        unregister_chrdev_region(dv_o, MN_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit ioctl_exit(void)
{
    device_destroy(cl, dv_o);
    class_destroy(cl);
    cdev_del(&dv_c);
    unregister_chrdev_region(dv_o, MN_CNT);
}

module_init(ioctl_init);
module_exit(ioctl_exit);

