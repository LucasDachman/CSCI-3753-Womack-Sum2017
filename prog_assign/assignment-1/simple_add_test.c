#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
	
	int num1 = 1;
	int num2 = 2;
	int result;
	int val;
	
	val = syscall(327, num1, num2, &result);
	printf("return val: %d\n", val);
	printf("result: %d\n", result);
		
	return 0;
}
