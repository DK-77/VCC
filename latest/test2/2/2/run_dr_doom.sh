#!/bin/bash


make 
sudo insmod io_driver_dr_doom.ko

# Launching the control station
./control_station &
c_pid=$!
echo "Control station PID: $c_pid"

# Launching the soldier
./soldier $c_pid &
echo "Soldier PID: $!"
sleep 2
kill -9 $c_pid

sudo rmmod io_driver_dr_doom