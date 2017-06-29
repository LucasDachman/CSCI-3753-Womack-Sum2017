/*
 * File: lookup.c
 * Author: Andy Sayler
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2012/02/01
 * Modify Date: 2012/02/01
 * Description:
 * 	This file contains the reference non-threaded
 *      solution to this assignment.
 *  
 */

/*
 * Revised by Lucas Dachman on 2017/06/28
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "util.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define MAX_NAME_LENGTH 1025

struct t_args {
	FILE* outputfp;
	char input_filename[MAX_NAME_LENGTH];
};

void* processInputFile(void* a) {
	struct t_args *args = a;
    FILE* inputfp = NULL;
    char firstipstr[INET6_ADDRSTRLEN];
    char errorstr[SBUFSIZE];
    char hostname[SBUFSIZE];

	/* Open Input File */
	inputfp = fopen(args->input_filename, "r");
	if(!inputfp){
		sprintf(errorstr, "Error Opening Input File: %s", args->input_filename);
		perror(errorstr);
		return;
	}	

	/* Read File and Process*/
	while(fscanf(inputfp, INPUTFS, hostname) > 0){

		/* Lookup hostname and get IP string */
		if(dnslookup(hostname, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE){
			fprintf(stderr, "dnslookup error: %s\n", hostname);
			strncpy(firstipstr, "", sizeof(firstipstr));
		}
		/* BEGIN CRITICAL SECTION */
		/* Write to Output File */
		fprintf(args->outputfp, "%s,%s\n", hostname, firstipstr);
		/* END CRITICAL SECTION */
	}
	/* Close Input File */
	fclose(inputfp);
	
}

int main(int argc, char* argv[]){

    /* Local Vars */
    FILE* outputfp = NULL;
	/* firstipstr is for first ip address returned by dnslookup */
    int i;
	int NUM_THREADS = argc-1;
	struct t_args *args;
	pthread_t threads[NUM_THREADS];
	int rc;
    
    /* Check Arguments */
    if(argc < MINARGS){
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
    }

    /* Open Output File */
	outputfp = fopen(argv[(argc-1)], "w");
   	if(!outputfp){
		perror("Error Opening Output File");
		return EXIT_FAILURE;
   	}

    /* Loop Through Input Files and create threads*/
    for(i=1; i<NUM_THREADS; i++){
		threads[i] = (pthread_t) malloc(sizeof(pthread_t));
		args = malloc(sizeof(*args));
		strcpy(args->input_filename, argv[i]);
		args->outputfp = outputfp;
		rc = pthread_create(&(threads[i]), NULL, &processInputFile, args);
		if( rc ) {
			printf("ERROR: pthread_create() returned val: %d\n", rc);
		}
		//processInputFile(argv[i], outputfp);	
	}
	/* Join threads */
	void **retval;	
	for(int i=0; i<NUM_THREADS; i++) {
		rc = pthread_join(threads[i], retval);
		if(retval && retval == PTHREAD_CANCELED)
			printf("Thread %d canceled", i);
		if( rc ) 
			printf("ERROR: pthread_join(%d) returned error: %s\n",i ,strerror(rc));
	}
    /* Close Output File */
    fclose(outputfp);

    return EXIT_SUCCESS;
}
