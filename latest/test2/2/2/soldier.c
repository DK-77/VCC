#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include "io_driver_dr_doom.h"



void close_driver(const char *dr_name, int dr_fd)
{

    int result = close(dr_fd);
    if (result == -1)
    {
        perror("ERROR: could not close driver");
    }
}

int open_driver(const char *dr_name)
{

    int dr_fd = open(dr_name, O_RDWR);
    if (dr_fd == -1)
    {
        perror("ERR: error opnening driver");
    }

    return dr_fd;
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Invalid Arguments - Usage: %s <parents_pid>\n", argv[0]);
        exit(1);
    }

    pid_t parent_pid = atoi(argv[1]);
    pid_t self_pid = getpid();

    const char *dr_name = "/dev/ioctl_dr_doom";
    int dr_fd = open_driver(dr_name);

    driver_arg_t args;
    args.parent_pid = parent_pid;
    args.self_pid = self_pid;

    int result = ioctl(dr_fd, CHANGE_PARENT, &args);
    if (result == -1)
    {
        perror("ERR: Call to ioctl failed");
    }

    close_driver(dr_name, dr_fd);

    return EXIT_SUCCESS;
}