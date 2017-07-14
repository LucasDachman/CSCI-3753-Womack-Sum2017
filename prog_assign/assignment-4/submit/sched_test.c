#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

#define USAGE "<Scheduling Policy> <Number of Processes>"
#define IO_BOUND_FILENAME "./rw"
#define CPU_BOUND_FILENAME "./calc_pi"

/* function prototypes */
void runIOBound();
void runCPUBound();
void runMixed();

/* global vars */
int NUM_PROC, pid;

int main(int argc, char * argv[]) {

	int policy;
    struct sched_param param;

	/* check params */
	if (argc < 2) {
		printf("USAGE: %s %s\n", argv[0], USAGE);
		printf("Policies: SCHED_OTHER, SCHED_FIFO, SCHED_RR\n");
		exit(EXIT_FAILURE);
	}

	/* set NUM_PROC */
	NUM_PROC = atoi(argv[2]);
	printf("Number of Processes = %d\n", NUM_PROC);
	if(NUM_PROC == 0){
	    fprintf(stderr, "Bad proc_num value\n");
	    exit(EXIT_FAILURE);
	}

	/* Set policy */
	if(!strcmp(argv[1], "SCHED_OTHER")){
	    policy = SCHED_OTHER;
	}
	else if(!strcmp(argv[1], "SCHED_FIFO")){
	    policy = SCHED_FIFO;
	}
	else if(!strcmp(argv[1], "SCHED_RR")){
	    policy = SCHED_RR;
	}
	else{
	    fprintf(stderr, "Unhandeled scheduling policy\n");
	    exit(EXIT_FAILURE);
	}


    /* Set process to max prioty for given scheduler */
    param.sched_priority = sched_get_priority_max(policy);
    
    /* Set new scheduler policy */
    fprintf(stdout, "Current Scheduling Policy: %d\n", sched_getscheduler(0));
    fprintf(stdout, "Setting Scheduling Policy to: %d\n", policy);
    if(sched_setscheduler(0, policy, &param)){
		perror("Error setting scheduler policy");
		exit(EXIT_FAILURE);
    }
    fprintf(stdout, "New Scheduling Policy: %d\n", sched_getscheduler(0));
	
	printf("\nStarting tests...\n\n");
	// start tests
	runIOBound();
	runCPUBound();
}

void runIOBound() {

	int i;
	char *const IO_args[] = {IO_BOUND_FILENAME, "-1", "-1", "-1", "-1", 0};
	printf("Staring IO Bound test...\n");

	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = fork();
	
		if( pid < 0 ) {
			// error forking
			perror("Error forking: ");
		}
		else if( pid == 0 ) {
			// child code
			execvp(IO_BOUND_FILENAME, IO_args);
			perror("Error executing IO_BOUND: ");
			break;
		}
	}
	if( pid > 0 ) {
		// parent code
		// wait for chilren to finish		
		int wstatus;
		for( i=0; i<NUM_PROC; i++ ) {
			if( wait(&wstatus) == -1 ) {
				perror("Error waiting: ");
			}
		}
		printf("Finished IO Bound test.\n\n");
	}

}

void runCPUBound() {

	int i;
	char *const CPU_args[] = {CPU_BOUND_FILENAME, "-1", 0};
	printf("Staring CPU Bound test...\n");

	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = fork();
	
		if( pid < 0 ) {
			// error forking
			perror("Error forking: ");
		}
		else if( pid == 0 ) {
			// child code
			execvp(CPU_BOUND_FILENAME, CPU_args);
			perror("Error executing CPU_BOUND: ");
			break;
		}
	}
	if( pid > 0 ) {
		// parent code
		// wait for chilren to finish		
		int wstatus;
		for( i=0; i<NUM_PROC; i++ ) {
			if( wait(&wstatus) == -1 ) {
				perror("Error waiting: ");
			}
		}
		printf("Finished CPU Bound test...\n\n");
	}

}
