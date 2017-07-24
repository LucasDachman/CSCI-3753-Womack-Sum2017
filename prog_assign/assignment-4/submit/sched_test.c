#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>

#define USAGE "<Scheduling Policy> <Number of Processes> <outputfile>"
#define IO_BOUND_FILENAME "./rw"
#define CPU_BOUND_FILENAME "./calc_pi"
#define MIXED_FILENAME "./rwpi"

/* function prototypes */
void runIOBound();
void runCPUBound();
void runMixed();
void print_rusage_info(struct rusage *rusage, int pid);

/* global vars */
int NUM_PROC, pid;
FILE * outputfp;

/* io bound proc */
struct timeval total_sys_time_io;
struct timeval total_usr_time_io;
long int num_voluntary_context_switches_io = 0;
long int num_involuntary_context_switches_io = 0;

/* cpu bound proc */
struct timeval total_sys_time_cpu;
struct timeval total_usr_time_cpu;
long int num_voluntary_context_switches_cpu = 0;
long int num_involuntary_context_switches_cpu = 0;

/* mixed proc */
struct timeval total_sys_time_mixed;
struct timeval total_usr_time_mixed;
long int num_voluntary_context_switches_mixed = 0;
long int num_involuntary_context_switches_mixed = 0;


int main(int argc, char * argv[]) {

	int policy;
    struct sched_param param;

	/* check params */
	if (argc < 4) {
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

	/* get output file */
	outputfp = fopen(argv[3], "w");
	if (outputfp == 0) {
		perror("Could not open output file");
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

	/* clear timevals */
	timerclear(&total_sys_time_io);
	timerclear(&total_sys_time_cpu);
	timerclear(&total_usr_time_io);
	timerclear(&total_usr_time_cpu);
	
	printf("\nStarting tests...\n\n");
	// start tests
	runIOBound();
	runCPUBound();
	runMixed();

	/* system time strings */
	char sys_time_io[10];
   	sprintf(sys_time_io, "%ld.%06ld", total_sys_time_io.tv_sec, total_sys_time_io.tv_usec);
	char sys_time_cpu[10];
   	sprintf(sys_time_cpu, "%ld.%06ld",total_sys_time_cpu.tv_sec, total_sys_time_cpu.tv_usec);
	char sys_time_mixed[10];
   	sprintf(sys_time_mixed, "%ld.%06ld",total_sys_time_mixed.tv_sec, total_sys_time_mixed.tv_usec);

	/* system time floats */
	double sys_time_io_f = atof(sys_time_io);
	double sys_time_cpu_f = atof(sys_time_cpu);
	double sys_time_mixed_f = atof(sys_time_mixed);

	/* user time strings */
	char usr_time_io[10];
   	sprintf(usr_time_io, "%ld.%06ld", total_usr_time_io.tv_sec, total_usr_time_io.tv_usec);
	char usr_time_cpu[10];
   	sprintf(usr_time_cpu, "%ld.%06ld", total_usr_time_cpu.tv_sec, total_usr_time_cpu.tv_usec);
	char usr_time_mixed[10];
   	sprintf(usr_time_mixed, "%ld.%06ld", total_usr_time_mixed.tv_sec, total_usr_time_mixed.tv_usec);
	
	/* user time floats */
	double usr_time_io_f = atof(usr_time_io);
	double usr_time_cpu_f = atof(usr_time_cpu);
	double usr_time_mixed_f = atof(usr_time_mixed);

	/* Total Times */
	double total_time_io = sys_time_io_f+usr_time_io_f;
	double total_time_cpu = sys_time_cpu_f+usr_time_cpu_f;
	double total_time_mixed = sys_time_mixed_f+usr_time_mixed_f;


	/* table output */
	//fprintf(outputfp, "\n%-20s%-20s%-20s\n", "##########", argv[3], "##########");
	fprintf(outputfp,
			"\n################################################################################\n");
	fprintf(outputfp, "Output Filename:\t%s\n", argv[3]);
	fprintf(outputfp, "Scheduling Policy:\t%s\n", argv[1]);
	fprintf(outputfp, "Number of Processes:\t%d\n", NUM_PROC);
	fprintf(outputfp, "%-20s%-20s%-20s%-20s\n", "Field", "IO", "CPU", "Mixed"); 
	fprintf(outputfp, "----------------------------------------------------"
					  "----------------------------\n");
	/* System Times */
	fprintf(outputfp, "%-20s%-20s%-20s%-20s\n",
					  "System Time:", sys_time_io, sys_time_cpu, sys_time_mixed); 
	/* User Times */
	fprintf(outputfp, "%-20s%-20s%-20s%-20s\n",
					  "User Time:", usr_time_io, usr_time_cpu, usr_time_mixed); 
	/* Total Times */
	fprintf(outputfp, "%-20s%-20f%-20f%-20f\n", 
					  "Total Time:", total_time_io, total_time_cpu, total_time_mixed);
	/* Average Times */
	fprintf(outputfp, "%-20s%-20f%-20f%-20f\n", 
					  "Average Time:", total_time_io/NUM_PROC, 
					  total_time_cpu/NUM_PROC, total_time_mixed/NUM_PROC);
	/* Voluntary Context Switches */
	fprintf(outputfp, "Voluntary\n");
	fprintf(outputfp, "%-20s%-20ld%-20ld%-20ld\n",
					  "Context Switches:", num_voluntary_context_switches_io,
					  num_voluntary_context_switches_cpu, num_voluntary_context_switches_mixed); 
	/* Average Voluntary Context Switches */
	fprintf(outputfp, "Average Voluntary\n");
	fprintf(outputfp, "%-20s%-20f%-20f%-20f\n",
					  "Context Switches:", (double)num_voluntary_context_switches_io/NUM_PROC,
					  (double)num_voluntary_context_switches_cpu/NUM_PROC, 
					  (double)num_voluntary_context_switches_mixed/NUM_PROC); 
	/* Involuntary Context Switches */
	fprintf(outputfp, "Involuntary\n");
	fprintf(outputfp, "%-20s%-20ld%-20ld%-20ld\n",
					  "Context Switches:", num_involuntary_context_switches_io,
					  num_involuntary_context_switches_cpu, num_involuntary_context_switches_mixed); 
	/* Average Involuntary Context Switches */
	fprintf(outputfp, "Average Involuntary\n");
	fprintf(outputfp, "%-20s%-20f%-20f%-20f\n",
					  "Context Switches:", (double)num_involuntary_context_switches_io/NUM_PROC,
					  (double)num_involuntary_context_switches_cpu/NUM_PROC,
					  (double)num_involuntary_context_switches_mixed/NUM_PROC); 
	fprintf(outputfp,
			"################################################################################\n");
}

void runIOBound() {

	int i;
	char *const IO_args[] = {IO_BOUND_FILENAME, "-1", "-1", "-1", "-1", 0};
	fprintf(outputfp, "Staring IO Bound test...\n");

	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = vfork();
	
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
		int wstatus, rv;
		struct rusage rusage;
		for( i=0; i<NUM_PROC; i++ ) {
			rv = wait3(&wstatus, 0, &rusage);
			if( rv == -1 ) {
				perror("Error waiting: ");
			} else {
				print_rusage_info(&rusage, rv);
				timeradd(&total_sys_time_io, &(rusage.ru_stime), &total_sys_time_io);
				timeradd(&total_usr_time_io, &(rusage.ru_utime), &total_usr_time_io);
				num_voluntary_context_switches_io += rusage.ru_nvcsw;
				num_involuntary_context_switches_io += rusage.ru_nivcsw;
			}
		}
		fprintf(outputfp, "Finished IO Bound test.\n\n");
	}

}

void runCPUBound() {

	int i;
	char *const CPU_args[] = {CPU_BOUND_FILENAME, "-1", 0};
	fprintf(outputfp, "Staring CPU Bound test...\n");

	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = vfork();
	
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
		int wstatus, rv;
		struct rusage rusage;
		for( i=0; i<NUM_PROC; i++ ) {
			rv = wait3(&wstatus, 0, &rusage);
			if( rv == -1 ) {
				perror("Error waiting: ");
			} else {
				print_rusage_info(&rusage, rv);
				timeradd(&total_sys_time_cpu, &(rusage.ru_stime), &total_sys_time_cpu);
				timeradd(&total_usr_time_cpu, &(rusage.ru_utime), &total_usr_time_cpu);
				num_voluntary_context_switches_cpu += rusage.ru_nvcsw;
				num_involuntary_context_switches_cpu += rusage.ru_nivcsw;
			}
		}
		fprintf(outputfp, "Finished CPU Bound test...\n\n");
	}

}

void runMixed() {

	int i;
	char *const MIXED_args[] = {MIXED_FILENAME, "-1", "-1", 0};
	fprintf(outputfp, "Staring Mixed test...\n");

	/* create NUM_PROC processes with fork */
	for( i=0; i<NUM_PROC; i++) {
		pid = vfork();
	
		if( pid < 0 ) {
			// error forking
			perror("Error forking: ");
		}
		else if( pid == 0 ) {
			// child code
			execvp(MIXED_FILENAME, MIXED_args);
			perror("Error executing MIXED: ");
			break;
		}
	}
	if( pid > 0 ) {
		// parent code
		// wait for chilren to finish		
		int wstatus, rv;
		struct rusage rusage;
		for( i=0; i<NUM_PROC; i++ ) {
			rv = wait3(&wstatus, 0, &rusage);
			if( rv == -1 ) {
				perror("Error waiting: ");
			} else {
				print_rusage_info(&rusage, rv);
				timeradd(&total_sys_time_mixed, &(rusage.ru_stime), &total_sys_time_mixed);
				timeradd(&total_usr_time_mixed, &(rusage.ru_utime), &total_usr_time_mixed);
				num_voluntary_context_switches_mixed += rusage.ru_nvcsw;
				num_involuntary_context_switches_mixed += rusage.ru_nivcsw;
			}
		}
		fprintf(outputfp, "Finished Mixed test...\n\n");
	}

}

void print_rusage_info(struct rusage *rusage, int pid) {

	fprintf(outputfp, "##### rusage for pid=%d #####\n", pid);
	fprintf(outputfp, "User Time: %ld.%06ld\n", rusage->ru_utime.tv_sec, rusage->ru_utime.tv_usec);
	fprintf(outputfp, "CPU Time: %ld.%06ld\n", rusage->ru_stime.tv_sec, rusage->ru_stime.tv_usec);
	fprintf(outputfp, "Full Swaps: %ld\n", rusage->ru_nswap);
	fprintf(outputfp, "FS Read from Disk: %ld\n", rusage->ru_inblock);
	fprintf(outputfp, "FS Write to Disk: %ld\n", rusage->ru_oublock);
	fprintf(outputfp, "Voluntary Context Switches: %ld\n", rusage->ru_nvcsw);
	fprintf(outputfp, "Involuntary Context Switches: %ld\n", rusage->ru_nivcsw);
	return;

}
