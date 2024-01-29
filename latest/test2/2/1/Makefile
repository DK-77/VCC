EXTRA_CFLAGS = -Wall -g

obj-m += ioctl_driver.o

PWD := $(CURDIR)

all: test_driver
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
test_driver: test_driver.c
	gcc test_driver.c -o test_driver

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test_driver