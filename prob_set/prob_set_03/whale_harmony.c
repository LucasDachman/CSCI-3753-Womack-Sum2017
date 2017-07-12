#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

/* function prototypes */
void* male(void*);
void* female(void*);
void* matchmaker(void*);

/* threads and semaphores */
sem_t m_sem, f_sem, mm_sem;
pthread_t m_thread, f_thread, mm_thread;

int main(int argc, char * argv[]) {
	
	int rc;
	sem_init(&m_sem, 0, 1);
	sem_init(&f_sem, 0, 1);
	sem_init(&mm_sem, 0, 1);

	/* create male thread */
	rc = pthread_create(&m_thread, 0, male, 0);
	if( rc ) { 
		fprintf(stderr, "Error, pthread_create() male\n");
		exit(-1);
	}

	/* create female thread */
	rc = pthread_create(&f_thread, 0, female, 0);
	if( rc ) { 
		fprintf(stderr, "Error, pthread_create() female\n");
		exit(-1);
	}
	
	/* create matchmaker thread */
	rc = pthread_create(&mm_thread, 0, matchmaker, 0);
	if( rc ) {
		fprintf(stderr, "Error, pthread_create() matchmaker\n");
		exit(-1);
	}

	/* join all threads */
	rc = pthread_join(m_thread, 0);
	if( rc ) {
		fprintf(stderr, "Error, pthread_join male\n");
		exit(-1);
	}
	
	rc = pthread_join(f_thread, 0);
	if( rc ) {
		fprintf(stderr, "Error, pthread_join female\n");
		exit(-1);
	}

	rc = pthread_join(mm_thread, 0);
	if( rc ) {
		fprintf(stderr, "Error, pthread_join matchmaker\n");
		exit(-1);
	}
	
	
}

void* male(void* args) {
	sem_post(&m_sem);
	sem_wait(&f_sem);
	sem_wait(&mm_sem);

}

void* female(void* args) {
	sem_post(&f_sem);
	sem_wait(&m_sem);
	sem_wait(&mm_sem);

}

void* matchmaker(void* args) {
	sem_wait(&m_sem);
	sem_wait(&f_sem);
	printf("A calf is born\n");
	sem_post(&mm_sem);

}
