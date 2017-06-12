/**
 * Figure 3.32
 *
 * To use this with the python script:
 *
 * (1) Compile this program:
 *          gcc loop-fork.c
 * (2) Send the output of this program to the python script:
 *          ./a.out | set.py
 * which outputs the number of unique process ids.
 */

#include <stdio.h>

int main()
{
	int i;

	for (i = 0; i < 4; i++) {
		fork();
		printf("%d\n",getpid());
	}
	
	return 0;
}
