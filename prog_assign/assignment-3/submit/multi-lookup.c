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
	queue hostname_q;
	pthread_t request_threads[NUM_REQUEST_THREADS];
	int rc;
	/* End local vars */
	/* Check Arguments */
	if( argc < MINARGS ) {
			fprintf(stderr, "Not enough arguments.\n");
			fprintf(stderr, "Usage:\n %s %s \n", argv[0], USAGE);
			return EXIT_FAILURE;
	}

	/* open output file */
	output_fp = fopen(argv[argc-1], "w");
	if(!output_fp) {
			perror("Error opening output file.");
			return EXIT_FAILURE;
	}
	
	/* set up queue */
	if( queue_init(&hostname_q, MAX_RESOLVER_THREADS) == QUEUE_FAILURE ) {
		perror("queue_init failure!");
	}
	struct request_thread_args *req_args;

	/* service name files */
	for(int i=0; i<NUM_NAME_FILES; i++) {
		req_args = (struct request_thread_args *) malloc(sizeof(struct request_thread_args));
		req_args->q = &hostname_q;
		req_args->filename = argv[i+1];
		/* Start request thread */
		rc = pthread_create(&request_threads[i], NULL, &service_name_file, (void *) req_args);
		if( rc ) 
			fprintf(stderr, "Failed creating thread [%d], strerror(): %s\n", i-1, strerror(rc));
	}



	/* Join Request Threads */
	for(int i=0; i<NUM_REQUEST_THREADS; i++) {
		rc = pthread_join(request_threads[i], NULL);
		if( rc ) 
			fprintf(stderr, "pthread_join(%d) returned error: %s\n", i, strerror(rc));
	}
	/* ##### TESTING if names are being added to queue ##### 
	for(int i=0; i<MAX_RESOLVER_THREADS; i++) {
		char *name = queue_pop(&hostname_q);
		printf("hostname: %s\n", name);
	}
	*/
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
	fclose(name_file_ptr);
	return NULL;

}
