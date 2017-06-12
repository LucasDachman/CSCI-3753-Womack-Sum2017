/**
 * filecopy.c
 * 
 * This program copies files using a pipe.
 *
 * Usage:
 *	filecopy <input file> <output file>
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
	int rv;
	pid_t pid;
	int c;
	char rb[2], wb[2];	/* bytes for reading/writing */
	int ffd[2];		/* file descriptor */
	int pfd[2];		/* pipe file descriptor */

	/* open the input file */
	ffd[READ_END] = open(argv[1], O_RDONLY);
	
	if (ffd[READ_END] < 0) {
		fprintf(stderr,"Unable to open %s\n",argv[1]);
		return 1;
	}
	
	/* open the output file */
	ffd[1] = open(argv[2], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if (ffd[1] < 0) {
		fprintf(stderr,"Unable to open %s\n",argv[2]);

		/* close the input file */
		close(ffd[0]);

		return 1;
	}

	/* set up the pipe */
     if (pipe(pfd) == -1) {
     	fprintf(stderr,"Pipe failed");

		/* close the open files */
		close(ffd[0]);
		close(ffd[1]);

		return 1;
	}

	/* now create the processes */
	pid = fork();
	
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");

		/* close the files */
		close(ffd[0]);
		close(ffd[1]);

		return 1;
	}

	if (pid > 0) {	/* parent */
		/* close the end of the pipe we do not need */
		close(pfd[READ_END]);

		/* read from the input file and write to the pipe */
		while ( (c = read(ffd[0], rb, 1)) > 0 ) {
			write(pfd[WRITE_END],rb, c);
		}

		/* close the read end of the input file and the write end of the pipe */
		close(ffd[0]);
		close(pfd[WRITE_END]);

		wait(NULL);
	}
	else { /* child */ 
		/* close the end of the pipe we do not need */
		close(pfd[WRITE_END]);

		/* read from the pipe and write to the output file */
		while ( (c = read(pfd[READ_END], rb, 1)) > 0) {
			write(ffd[1], rb, c);
		}

		/* close the write end of the input file and the read end of the pipe */
		close(ffd[1]);
		close(pfd[READ_END]);
	}

	return 0;
}
