#!/usr/bin/env bash
make -C /lib/modules/4.4.67lucas-revision-1/build M=/home/user/CSCI-3753-Womack-Sum2017/prog_assign/assignment-2/module2 modules
echo "continue? (y/n)"
read cont
if [ $name="y" ]; then
	sudo rmmod char_module.ko
	sudo rm /dev/char_device
	sudo insmod char_module.ko
	sudo mknod -m 777 /dev/char_device c 246 0
fi	

