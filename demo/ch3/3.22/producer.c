/**
 * This is the producer process that writes the Collatz 
 * sequence to the shared memory region.
 *
 * Operating System Concepts, Ninth Edition
 * Copyright John Wiley & Sons, 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	const int SIZE = 4096;
	const char *name = "COLLATZ";

	int shm_fd;
	void *ptr;
	char str[128];
	int n = atoi(argv[1]);

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/**
	 * Now write the Collatz sequence to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */

	sprintf(str,"%d, ", n);
	sprintf(ptr,"%s",str);
	while (n != 1) {
		ptr += strlen(str);

		if (n % 2 == 0)
			n = n / 2;
		else
			n = 3 * n + 1;

		sprintf(str,"%d, ", n);
		sprintf(ptr,"%s",str);
	}

	return 0;
}
