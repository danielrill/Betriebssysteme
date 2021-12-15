#include "concurrent_counter.h"
#include "measure_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define MAGIC 100

typedef struct thr_fn {
	concurrent_counter_t *counter;
	pthread_t thread;

	int cpu_nr;
} thr_fn;

void printids(const char *s) {
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();

	printf("( %s ) pid ( %lu ) | tid ( %lu )\n", s, (unsigned long) pid, (unsigned long) tid);
}


void *job(void *args) {
	printids("worker ->");
	concurrent_counter_t *tmp = (concurrent_counter_t*) args;
	for( int i = 0; i < MAGIC; i++)
	{
		increment(tmp);
	}

	return ((void*) 0);
}


int main(){
	int cpus = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if (cpus == -1)
		printf("Error no CPU detected");

	pthread_t t1, t2, t3, t4;
	concurrent_counter_t counter;
	concurrent_counter_t *dynamic_counter = malloc(sizeof(concurrent_counter_t));
	assert(dynamic_counter != NULL);

	// STATISCHER COUNTER
	init(&counter);
	pthread_create(&t1, NULL, job, &counter);
	pthread_join(t1, NULL);
	pthread_create(&t2, NULL, job, &counter);
	pthread_join(t2, NULL);

	printf("\nstatic value %d\n\n", get(&counter));

	// DYNAMISCHER COUNTER
	init(dynamic_counter);
	pthread_create(&t3, NULL, job, dynamic_counter);
	pthread_join(t3, NULL);
	pthread_create(&t4, NULL, job, dynamic_counter);
	pthread_join(t4, NULL);

	printf("dynamic value %d\n\n", get(dynamic_counter));

	printids("main -> ");

	free(dynamic_counter);
	exit(0);
}

