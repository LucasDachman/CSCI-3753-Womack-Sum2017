#!/usr/bin/env bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
echo "gcc test_char_driver.c -o test" ; gcc test_char_driver.c -o test
echo "continue to install device? (y/n)"
read cont
if [ "$cont" == "y" ]; then
	echo "sudo rmmod char_module.ko" ; sudo rmmod char_module.ko
	echo "sudo rm /dev/char_device" ; sudo rm /dev/char_device
	echo "sudo insmod char_module.ko" ; sudo insmod char_module.ko
	echo "Enter device major number (check dmesg): "
	read major
	echo "sudo mknod -m 777 /dev/char_device c $major 0" ; sudo mknod -m 777 /dev/char_device c $major 0
fi	

