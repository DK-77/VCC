#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>

typedef struct
{
    unsigned long va, pa;
    char val;
} driver_arg_t;

#define DRIVER_GET_PHYSADDR _IOWR('q', 1, driver_arg_t *)
#define DRIVER_SET_VAL _IOW('q', 2, driver_arg_t *)

#endif