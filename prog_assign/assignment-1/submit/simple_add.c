#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage long sys_simple_add(int num1, int num2, int * result) {
	*result = num1+num2;
	printk(KERN_ALERT "num1: %d\n", num1);
	printk(KERN_ALERT "num2: %d\n", num2);
	printk(KERN_ALERT "result: %d\n", *result);
	return 0;	
}
