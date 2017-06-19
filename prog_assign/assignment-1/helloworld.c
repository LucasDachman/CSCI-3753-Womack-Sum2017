#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

	int val;	
	val = syscall(326);
	printf("return val: %d\n", val);
	return 0;
}
