#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "ioctl_driver.h"

unsigned long pa = 0;

void set_val_to_pa(int fd, unsigned long pa, char a){
    
    driver_arg_t darg;
    darg.pa = pa;
    darg.val = a;

    if (ioctl(fd, DRIVER_SET_VAL, &darg) == -1){
        perror("error in set");
    }
}

void get_pa(int fd, unsigned long va){
    
    driver_arg_t darg;
    darg.va = va;

    if (ioctl(fd, DRIVER_GET_PHYSADDR, &darg) == -1){
        perror("error in get");
    }
    else{
        pa = darg.pa;
        printf("The physical address of the memory is assigned PA: 0x%lx\n", darg.pa);
    }
}

int main()
{
    char *ptr = (char *)malloc(1);
    printf("allocated a byte on the heap\n");
    if (ptr == NULL){
        printf("malloc failed\n");
        return 1;
    }

    *ptr = '6';
    printf("Assigned value “6” to the memory\n");
    unsigned long va = (unsigned long)ptr;
    printf("The virtual address of the memory VA: 0x%lx\n", va);

    int fd;
    char *file_name = "/dev/driver";

    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("err in open");
        return 2;
    }

    printf("Made ioctl() call for physical address\n");
    get_pa(fd, va);

    char new_val = '5';
    printf("Made ioctl() call to change value present at physical address received above\n");
    set_val_to_pa(fd, pa, new_val);

    printf("Modified Byte value: %c\n", *ptr);

    close(fd);
    free(ptr);
    return 0;
}