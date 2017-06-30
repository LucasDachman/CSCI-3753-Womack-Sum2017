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
#include <unistd.h>
#include <stdbool.h>

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
void *process_host_names(void *args);

/* Global definitions */
struct request_thread_args {
	queue * q;
	char * filename;
};
struct resolver_thread_args {
	queue * q;
	FILE * fileptr;
};

/* Synchronization stuff */
pthread_cond_t q_has_item;
pthread_mutex_t q_lock;
pthread_mutex_t output_file_lock;
pthread_mutex_t finished_requests_lock;
bool finished_requests = 0;


int main(int argc, char* argv[]) {

	/* pthread init stuff */
	pthread_cond_init(&q_has_item, NULL);
	pthread_mutex_init(&q_lock, NULL);
	pthread_mutex_init(&output_file_lock, NULL);
	pthread_mutex_init(&finished_requests_lock, NULL);

	/* Begin local vars */
	FILE* output_fp = NULL;
	int NUM_NAME_FILES = argc-2;
	int NUM_REQUEST_THREADS = argc-2;
	queue hostname_q;
	pthread_t request_threads[NUM_REQUEST_THREADS];
	pthread_t resolver_threads[MAX_RESOLVER_THREADS];
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

	/* service name files */
	struct request_thread_args *req_args;
	for(int i=0; i<NUM_NAME_FILES; i++) {
		req_args = (struct request_thread_args *) malloc(sizeof(struct request_thread_args));
		req_args->q = &hostname_q;
		req_args->filename = argv[i+1];
		/* Start request thread */
		rc = pthread_create(&request_threads[i], NULL, &service_name_file, (void *) req_args);
		if( rc ) 
			fprintf(stderr,"Failed creating request thread [%d], strerror(): %s\n",i-1,strerror(rc));
	}

	/* process host names */
	struct resolver_thread_args *res_args;
	for(int i=0; i<MAX_RESOLVER_THREADS; i++) {
		res_args = (struct resolver_thread_args *) malloc(sizeof(struct resolver_thread_args));
		res_args->fileptr = output_fp;
		res_args->q = &hostname_q;
		rc = pthread_create(&resolver_threads[i], NULL, &process_host_names, (void *) res_args);
		if( rc ) 
			fprintf(stderr,"Failed creating resolver thread[%d], strerror(): %s\n",i-1,strerror(rc));
	}	

	/* Join Request Threads */
	for(int i=0; i<NUM_REQUEST_THREADS; i++) {
		rc = pthread_join(request_threads[i], NULL);
		if( rc ) 
			fprintf(stderr,
					"pthread_join(%d) (request thread) returned error: %s\n", i, strerror(rc));
	}
	/* All request threads are finished.
	 * signal resolver threads to stop popping off queue
	 */
	pthread_mutex_lock(&finished_requests_lock);
	finished_requests = 1;
	pthread_mutex_unlock(&finished_requests_lock);

	/* Join Resolver Threads */
	for(int i=0; i<MAX_RESOLVER_THREADS; i++) {
		rc = pthread_join(resolver_threads[i], NULL);
		if( rc ) {
			fprintf(stderr,
					"pthread_join(%d) (resolver thread) returned error: %s\n", i, strerror(rc));
		}
	}
	fclose(output_fp);

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

	/* open name file */
	FILE * name_file_ptr = fopen(filename, "r");
	if( !name_file_ptr ) {
		fprintf(stderr, "Error opening name file [%s]", filename);
		pthread_exit(NULL);
	}

	/* Read file and add names to queue */
	while( fscanf(name_file_ptr, HOST_NAME_FORMAT, hostname) > 0 ) {

		/* CRITICAL SECTION BEGINS */
		pthread_mutex_lock(&q_lock);
		while( queue_is_full(q) ) {
			pthread_mutex_unlock(&q_lock);
			// sleep for random(0,100) microseconds)
			unsigned int usecs;
			usecs = rand() % 101;
			usleep(usecs);
			pthread_mutex_lock(&q_lock);
		}
		char * hostname_ptr = malloc(sizeof(hostname));
		strcpy(hostname_ptr, hostname);
		queue_push(q,(void *) hostname_ptr);
		pthread_cond_signal(&q_has_item);
		pthread_mutex_unlock(&q_lock);
		/* CRITICAL SECTION ENDS */

	}
	fclose(name_file_ptr);
	pthread_exit(NULL);
}

 /* Resolver thread */

/* 
 * Takes name off hostname queue and querries the IP
 * Output is written to given output file
 *
 */

void *process_host_names(void *args) {
	
	char ipstr[INET_ADDRSTRLEN];
	struct resolver_thread_args *res_args;
	res_args = (struct resolver_thread_args *) args;
	queue *q = res_args->q;
	FILE * fileptr = res_args->fileptr;
	pthread_mutex_lock(&finished_requests_lock);
	while( !finished_requests ) {	
		/* aquire queue lock */
		pthread_mutex_lock(&q_lock);
		/* wait for signal that q is not empty */
		while( queue_is_empty(q) && !finished_requests) {
			pthread_mutex_unlock(&finished_requests_lock);
			pthread_mutex_unlock(&q_lock);
			usleep(100);
			pthread_mutex_lock(&finished_requests_lock);
			pthread_mutex_lock(&q_lock);
			//pthread_cond_wait(&q_has_item, &q_lock);
		}
		pthread_mutex_unlock(&finished_requests_lock);
		/* lookup hostname and get IP string */
		char *hostname = queue_pop(q);
		/* release queue lock */
		pthread_mutex_unlock(&q_lock);
		if( dnslookup(hostname, ipstr, sizeof(ipstr)) == UTIL_FAILURE ) {
			fprintf(stderr, "dnslookup error: %s\n", hostname);
			strncpy(ipstr, "", sizeof(ipstr));
		}
		
		/* aquire output file lock */
		pthread_mutex_lock(&output_file_lock);
		fprintf(fileptr, "%s,%s\n", hostname, ipstr);
		/* release output file lock */
		pthread_mutex_unlock(&output_file_lock);

		pthread_mutex_lock(&finished_requests_lock);
	}	
	pthread_mutex_unlock(&finished_requests_lock);
	
	pthread_exit(NULL);

}

