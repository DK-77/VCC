#include <linux/ioctl.h>

typedef struct
{
    pid_t parent_pid;
    pid_t self_pid;
} driver_arg_t;

#define CHANGE_PARENT _IOW('q', 1, driver_arg_t *)