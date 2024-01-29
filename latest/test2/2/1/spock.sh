#!/bin/sh
make
sudo insmod ioctl_driver.ko 
sudo ./test_driver
sudo rmmod ioctl_driver
make clean