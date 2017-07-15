/*
 * File: pi-sched.c
 * Author: Andy Sayler
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2012/03/07
 * Modify Date: 2012/03/09
 * Description:
 * 	This file contains a simple program for statistically
 *      calculating pi using a specific scheduling policy.
 */

/**
 * Modified by Lucas Dachman on 2017/07/12
 *
 */

/* Local Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEFAULT_ITERATIONS 1000000
#define RADIUS (RAND_MAX / 2)

double dist(double x0, double y0, double x1, double y1){
    return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
}

double zeroDist(double x, double y){
    return dist(0, 0, x, y);
}

void calc();

/* global vars */
long iterations;

int main(int argc, char* argv[]){

	int NUM_PROC = 1;
    struct sched_param param;
    int policy;
	int i;
	pid_t pid;

	/* check params */
	if( argc < 3 ) {
		printf("USAGE: %s <iterations> <policy> <num proc>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// set iterations
    iterations = atol(argv[1]);
	if(iterations < 1){
	    fprintf(stderr, "Bad iterations value\n");
	    exit(EXIT_FAILURE);
	}
	/* Set policy */
	if(!strcmp(argv[2], "SCHED_OTHER")){
	    policy = SCHED_OTHER;
	}
	else if(!strcmp(argv[2], "SCHED_FIFO")){
	    policy = SCHED_FIFO;
	}
	else if(!strcmp(argv[2], "SCHED_RR")){
	    policy = SCHED_RR;
	}
	else{
	    fprintf(stderr, "Unhandeled scheduling policy\n");
	    exit(EXIT_FAILURE);
	}

	/* set NUM_PROC */
	NUM_PROC = atoi(argv[3]);
	printf("NUM_PROC = %d\n", NUM_PROC);
	if(NUM_PROC == 0){
	    fprintf(stderr, "Bad proc_num value\n");
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
	
	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = fork();
	
		if( pid < 0 ) {
			// error forking
			perror("Error forking: ");
		}
		else if( pid == 0 ) {
			// child code
			calc();
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
	}

    return 0;
}


void calc() {

	long i;
	double x, y;
    double inCircle = 0.0;
    double inSquare = 0.0;
    double pCircle = 0.0;
    double piCalc = 0.0;


	/* Calculate pi using statistical methode across all iterations*/
    for(i=0; i<iterations; i++){
	x = (random() % (RADIUS * 2)) - RADIUS;
	y = (random() % (RADIUS * 2)) - RADIUS;
	if(zeroDist(x,y) < RADIUS){
	    inCircle++;
	}
	inSquare++;
    }

    /* Finish calculation */
    pCircle = inCircle/inSquare;
    piCalc = pCircle * 4.0;

    /* Print result */
    fprintf(stdout, "pi = %f\n", piCalc);

	return;

}
