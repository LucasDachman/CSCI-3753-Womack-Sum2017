/*
 * File: rw.c
 * Author: Andy Sayler
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2012/03/19
 * Modify Date: 2012/03/20
 * Description: A small i/o bound program to copy N bytes from an input
 *              file to an output file. May read the input file multiple
 *              times if N is larger than the size of the input file.
 */

/**
 * Revised by Lucas Dachman on 2017/07/12
 */

/* Include Flags */
#define _GNU_SOURCE

/* System Includes */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Local Defines */
#define MAXFILENAMELENGTH 80
#define DEFAULT_INPUTFILENAME "rwinput"
#define DEFAULT_OUTPUTFILENAMEBASE "rwoutput"
#define DEFAULT_BLOCKSIZE 1024
#define DEFAULT_TRANSFERSIZE 1024*100
#define DEFAULT_NUM_PROC 1
#define USAGE "<transfersize> <blocksize> <inputfilename> <outputfilenameBase> <num proc>"

/* function declarations */
void write_to_file();

/* Global Variables */
char outputFilenameBase[MAXFILENAMELENGTH];
char inputFilename[MAXFILENAMELENGTH];
int inputFD;
ssize_t buffersize;
ssize_t blocksize = 0;
ssize_t transfersize = 0;
char *transferBuffer = NULL;

int main(int argc, char *argv[])
{

	int i, pid, NUM_PROC;

	/* Process program arguments to select run-time parameters */
	/* check params */
	if (argc < 5) {
		printf("USAGE: %s %s\n", argv[0], USAGE);
		exit(EXIT_FAILURE);
	}
	

	/* Set supplied transfer size or default if not supplied */
	transfersize = atol(argv[1]);
	if (transfersize < 1) {
		transfersize = DEFAULT_TRANSFERSIZE;
		printf("Transfer Size set to default: %lu\n", transfersize);
	}
	
	blocksize = atol(argv[2]);
	if (blocksize < 1) {
		blocksize = DEFAULT_BLOCKSIZE;
		printf("Block Size set to default: %lu\n", blocksize);
	}
	
	/* Set supplied input filename or default if not supplied */
	if (strnlen(argv[3], MAXFILENAMELENGTH) >= MAXFILENAMELENGTH) {
		fprintf(stderr, "Input filename too long\n");
		exit(EXIT_FAILURE);
	}
	strncpy(inputFilename, argv[3], MAXFILENAMELENGTH);
	if (strcmp(inputFilename, "-1") == 0) {
		strncpy(inputFilename, DEFAULT_INPUTFILENAME, MAXFILENAMELENGTH);
		printf("Input Filename set to default: %s\n", inputFilename);
	}

	/* Set supplied output filename base or default if not supplied */
	if (strnlen(argv[4], MAXFILENAMELENGTH) >= MAXFILENAMELENGTH) {
		fprintf(stderr, "Output filename base is too long\n");
		exit(EXIT_FAILURE);
	}
	strncpy(outputFilenameBase, argv[4], MAXFILENAMELENGTH);
	if (strcmp(outputFilenameBase, "-1") == 0) {
		strncpy(outputFilenameBase, DEFAULT_OUTPUTFILENAMEBASE,MAXFILENAMELENGTH);
		printf("Output Filename Base set to default: %s\n", outputFilenameBase);
	}

	/* set NUM_PROC */
	NUM_PROC = atoi(argv[5]);
	if (NUM_PROC < 1) {
		NUM_PROC = DEFAULT_NUM_PROC;
	    printf("Number of Processes set to default: %d\n", NUM_PROC);
	}

	/* Confirm blocksize is multiple of and less than transfersize */
	if (blocksize > transfersize) {
		fprintf(stderr, "blocksize can not exceed transfersize\n");
		exit(EXIT_FAILURE);
	}
	if (transfersize % blocksize) {
		fprintf(stderr, "blocksize must be multiple of transfersize\n");
		exit(EXIT_FAILURE);
	}

	/* Allocate buffer space */
	buffersize = blocksize;
	if (!(transferBuffer = malloc(buffersize * sizeof(*transferBuffer)))) {
		perror("Failed to allocate transfer buffer");
		exit(EXIT_FAILURE);
	}

	/* Open Input File Descriptor in Read Only mode */
	if ((inputFD = open(inputFilename, O_RDONLY | O_SYNC)) < 0) {
		perror("Failed to open input file");
		exit(EXIT_FAILURE);
	}

	/* create NUM_PROC processes with fork */
	for (i=0; i<NUM_PROC; i++) {
		pid = fork();
	
		if( pid < 0 ) {
			// error forking
			perror("Error forking: ");
		}
		else if( pid == 0 ) {
			// child code
			// CALL WRITETOFILE
			write_to_file();
			break;
		}
	}
	if (pid > 0) {
		// parent code
		// wait for chilren to finish		
		int wstatus;
		for( i=0; i<NUM_PROC; i++ ) {
			if( wait(&wstatus) == -1 ) {
				perror("Error waiting: ");
			}
		}
	}
	
	/* Close Input File Descriptor */
	if (close(inputFD)) {
		perror("Failed to close input file");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void write_to_file() {

	int rv;
	int outputFD;
	char outputFilename[MAXFILENAMELENGTH];

	ssize_t bytesRead = 0;
	ssize_t totalBytesRead = 0;
	int totalReads = 0;
	ssize_t bytesWritten = 0;
	ssize_t totalBytesWritten = 0;
	int totalWrites = 0;
	int inputFileResets = 0;

	/* Open Output File Descriptor in Write Only mode with standard permissions */
	rv = snprintf(outputFilename, MAXFILENAMELENGTH, "%s-%d",
				  outputFilenameBase, getpid());
	if (rv > MAXFILENAMELENGTH) {
		fprintf(stderr,
				"Output filenmae length exceeds limit of %d characters.\n",
				MAXFILENAMELENGTH);
		exit(EXIT_FAILURE);
	} else if (rv < 0) {
		perror("Failed to generate output filename");
		exit(EXIT_FAILURE);
	}
	if ((outputFD =
		 open(outputFilename,
			  O_WRONLY | O_CREAT | O_TRUNC | O_SYNC,
			  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) < 0) {
		perror("Failed to open output file");
		exit(EXIT_FAILURE);
	}

	/* Print Status */
	fprintf(stdout, "Reading from %s and writing to %s\n",
			inputFilename, outputFilename);

	/* Read from input file and write to output file */
	do {
		/* Read transfersize bytes from input file */
		bytesRead = read(inputFD, transferBuffer, buffersize);
		if (bytesRead < 0) {
			perror("Error reading input file");
			exit(EXIT_FAILURE);
		} else {
			totalBytesRead += bytesRead;
			totalReads++;
		}

		/* If all bytes were read, write to output file */
		if (bytesRead == blocksize) {
			bytesWritten = write(outputFD, transferBuffer, bytesRead);
			if (bytesWritten < 0) {
				perror("Error writing output file");
				exit(EXIT_FAILURE);
			} else {
				totalBytesWritten += bytesWritten;
				totalWrites++;
			}
		}
		/* Otherwise assume we have reached the end of the input file and reset */
		else {
			if (lseek(inputFD, 0, SEEK_SET)) {
				perror("Error resetting to beginning of file");
				exit(EXIT_FAILURE);
			}
			inputFileResets++;
		}

	}
	while (totalBytesWritten < transfersize);

	/* Output some possibly helpfull info to make it seem like we were doing stuff */
	fprintf(stdout, "\n");
	fprintf(stdout, "pid:\t\t%d\n", getpid());
	fprintf(stdout, "Read:\t\t%zd bytes in %d reads\n",
			totalBytesRead, totalReads);
	fprintf(stdout, "Written:\t%zd bytes in %d writes\n",
			totalBytesWritten, totalWrites);
	fprintf(stdout, "Read input file in %d pass%s\n",
			(inputFileResets + 1), (inputFileResets ? "es" : ""));
	fprintf(stdout, "Processed %zd bytes in blocks of %zd bytes\n",
			transfersize, blocksize);

	/* Free Buffer */
	free(transferBuffer);

	/* Close Output File Descriptor */
	if (close(outputFD)) {
		perror("Failed to close output file");
		exit(EXIT_FAILURE);
	}


}
