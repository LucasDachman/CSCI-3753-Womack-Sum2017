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

/* Local Defines */
#define MAXFILENAMELENGTH 80
#define DEFAULT_INPUTFILENAME "/dev/urandom"
#define DEFAULT_OUTPUTFILENAMEBASE "rwoutput"
#define DEFAULT_OUTPUTFILENAME "/dev/null"
#define DEFAULT_BLOCKSIZE 1024
#define DEFAULT_TRANSFERSIZE 1024*100
#define USAGE "<transfersize> <blocksize> <inputfilename> <outputfilenameBase>"

/* Global Variables */
int main(int argc, char *argv[])
{

	char inputFilename[MAXFILENAMELENGTH];
	char outputFilename[MAXFILENAMELENGTH];
	int inputFD;
	ssize_t buffersize;
	ssize_t blocksize = 0;
	ssize_t transfersize = 0;
	char *transferBuffer = NULL;

	int rv;
	int outputFD;

	char tempName[MAXFILENAMELENGTH];
	ssize_t bytesRead = 0;
	ssize_t totalBytesRead = 0;
	int totalReads = 0;
	ssize_t bytesWritten = 0;
	ssize_t totalBytesWritten = 0;
	int totalWrites = 0;
	int inputFileResets = 0;

	/* Process program arguments to select run-time parameters */
	/* check params */
	if( argc > 1 && strcmp(argv[1], "-h") == 0 ) {
		fprintf(stdout,"Use -1 to set a parameter to its default.\n"
					   "defaults:\n"
					   "transfersize: %d\n"
					   "blocksize: %d\n"
					   "inputfilename: %s\n"
					   "outputfilename: %s\n",
					   DEFAULT_TRANSFERSIZE, DEFAULT_BLOCKSIZE, DEFAULT_INPUTFILENAME,
					   DEFAULT_OUTPUTFILENAME);

	}
	if (argc < 4) {
		printf("USAGE: %s %s\n", argv[0], USAGE);
		printf("%s -h for hep\n", argv[0]);
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
	strncpy(outputFilename, argv[4], MAXFILENAMELENGTH);
	if (strcmp(outputFilename, "-1") == 0) {
		strncpy(outputFilename, DEFAULT_OUTPUTFILENAME,MAXFILENAMELENGTH);
		printf("Output Filename set to default: %s\n", outputFilename);
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

	/* set outputfile name */
	strcpy(tempName, outputFilename);
	if (strcmp(outputFilename, DEFAULT_OUTPUTFILENAME) != 0) {
		rv = snprintf(outputFilename, MAXFILENAMELENGTH, "%s-%d",
				  tempName, getpid());
		
		if (rv > MAXFILENAMELENGTH) {
			fprintf(stderr,
					"Output filenmae length exceeds limit of %d characters.\n",
					MAXFILENAMELENGTH);
			exit(EXIT_FAILURE);
		} else if (rv < 0) {
			perror("Failed to generate output filename");
			exit(EXIT_FAILURE);
		}
	}
	/* Open Output File Descriptor in Write Only mode with standard permissions */
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
	fprintf(stdout, "outputfile:\t%s\n", outputFilename);
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
	
	/* Close Input File Descriptor */
	if (close(inputFD)) {
		perror("Failed to close input file");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void write_to_file() {



}
