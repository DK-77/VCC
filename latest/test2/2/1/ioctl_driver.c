#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <asm/pgtable.h>
#include <asm/io.h>
#include "ioctl_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv K");
MODULE_DESCRIPTION("IOCTL Driver");

#define MINOR_CNT 1
#define FIRST_MINOR 0

static dev_t dv_o;
static struct cdev dv_c;
static struct class *cl;

static int new_open(struct inode *i, struct file *f)
{
    return 0;
}

static void set_val_to_pa(unsigned long pa, char a)
{
    unsigned long va = (unsigned long)__va(pa);
    if (va)
    {
        *(char *)va = a;
    }
    return;
}
static unsigned long get_pa(unsigned long va)
{
    unsigned long pa = __pa(va);
    return pa;
}

static int new_close(struct inode *i, struct file *f)
{
    return 0;
}
static long new_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    driver_arg_t q;

    switch (cmd)
    {
    case DRIVER_GET_PHYSADDR:
        if (copy_from_user(&q, (driver_arg_t *)arg, sizeof(driver_arg_t)))
        {
            return -EACCES;
        }
        q.pa = get_pa(q.va);
        if (copy_to_user((driver_arg_t *)arg, &q, sizeof(driver_arg_t)))
        {
            return -EACCES;
        }
        break;

    case DRIVER_SET_VAL:
        if (copy_from_user(&q, (driver_arg_t *)arg, sizeof(driver_arg_t)))
        {
            return -EACCES;
        }
        set_val_to_pa(q.pa, q.val);
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
    int ret = 0;
    struct device *dev_ret;

    printk( "Initializing driver module\n");
    if ((ret = alloc_chrdev_region(&dv_o, FIRST_MINOR, MINOR_CNT, "driver_ioctl")) < 0)
    {
        return ret;
    }

    cdev_init(&dv_c, &driver_fops);

    if ((ret = cdev_add(&dv_c, dv_o, MINOR_CNT)) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&dv_c);
        unregister_chrdev_region(dv_o, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dv_o, NULL, "driver")))
    {
        class_destroy(cl);
        cdev_del(&dv_c);
        unregister_chrdev_region(dv_o, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit ioctl_exit(void)
{
    printk("Exiting driver module\n");
    device_destroy(cl, dv_o);
    class_destroy(cl);
    cdev_del(&dv_c);
    unregister_chrdev_region(dv_o, MINOR_CNT);
}

module_init(ioctl_init);
module_exit(ioctl_exit);

