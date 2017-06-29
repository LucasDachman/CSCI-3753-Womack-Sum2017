/* 
 * Author: Lucas Dachman
 * CSCI 3753 PA3
 *
 *
 * Some parts taken from Andy Saylors non-threaded solution
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "util.h" //dns lookup 
#include "queue.h" //FIFO queue

#define MINARGS 3
#define USAGE "<unputFilePath> <outputFlePath>"

#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 2
#define MAX_NAME_LENGTH 1024
#define HOST_NAME_FORMAT "%1024s"
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

/* Function prototypes */
void *service_name_file(void *args);

/* Global definitions */
struct request_thread_args {
		queue * q;
		char * filename;
};

int main(int argc, char* argv[]) {

	/* Begin local vars */
	FILE* output_fp = NULL;
	int NUM_NAME_FILES = argc-2;
	int NUM_REQUEST_THREADS = argc-2;
	queue fname_q;
	pthread_t request_threads[MAX_RESOLVER_THREADS];
	int rc;
	/* End local vars */
	/* Check Arguments */
	if( argc < MINARGS ) {
			perror("Not enough arguments.");
			fprintf(stderr, "Usage:\n %s %s \n", argv[0], USAGE);
	}

	/* open output file */
	output_fp = fopen(argv[argc-1], "w");
	if(!output_fp) {
			perror("Error opening output file.");
			return EXIT_FAILURE;
	}
	
	/* set up queue */
	if( queue_init(&fname_q, MAX_RESOLVER_THREADS) == QUEUE_FAILURE ) {
		perror("queue_init failure!");
	}
	struct request_thread_args *req_args;

	/* service name files */
	for(int i=1; i<NUM_REQUEST_THREADS; i++) {
		req_args = (struct request_thread_args *) malloc(sizeof(struct request_thread_args));
		req_args->q = &fname_q;
		req_args->filename = argv[i];
		/* Start request thread */
		rc = pthread_create(&request_threads[i-1], NULL, &service_name_file, (void *) req_args);
		if( rc ) 
			fprintf(stderr, "Failed creating thread [%d], strerror(): %s\n", i-1, strerror(rc));
	}



	/* Join Request Threads */
	for(int i=0; i<NUM_NAME
}

/* Request thread */
void *service_name_file(void *args) {

	/* set up local vars */
	struct request_thread_args *req_args;
	req_args = (struct request_thread_args *) args;
	queue * q = req_args->q;
	char * filename = req_args->filename;
	char hostname[MAX_NAME_LENGTH];
	char ipstring[INET6_ADDRSTRLEN];

	/* open name file */
	FILE * name_file_ptr = fopen(filename, "r");
	if( !name_file_ptr ) {
		fprintf(stderr, "Error opening name file [%s]", filename);
		return NULL;
	}

	/* Read file and add names to queue */
	while( fscanf(name_file_ptr, HOST_NAME_FORMAT, hostname) > 0 ) {

		/* Lookup hostname */
		if( dnslookup(hostname, ipstring, sizeof(ipstring)) == UTIL_FAILURE ) {
			fprintf(stderr, "dnslookup error: %s\n", hostname);
		}
		/* CRITICAL SECTION BEGINS */
		while( queue_is_full(q) ) {
			// sleep for random(0,100) microseconds)
		}
		char * hostname_ptr = malloc(sizeof(hostname));
		strcpy(hostname_ptr, hostname);
		queue_push(q,(void *) hostname_ptr);
		/* CRITICAL SECTION ENDS */

	}

	return NULL;

}
