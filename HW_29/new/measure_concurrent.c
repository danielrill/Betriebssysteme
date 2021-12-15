#define _GNU_SOURCE
#include <sched.h>

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
	thr_fn *tmp = (thr_fn*) args;
	cpu_set_t cpuset;
	CPU_SET(tmp -> cpu_nr, &cpuset);
	pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

	for( int i = 0; i < MAGIC; i++)
	{
		increment(tmp-> counter);
	}

	return ((void*) 0);
}


int main(){
	int cpus = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if (cpus == -1)
		printf("Error no CPU detected");

	for (int i= 1; i<=cpus; i++)
	{
		for (int j = 1; j <= cpus; j++)
		{
			thr_fn *thr = malloc((size_t)j * sizeof(thr_fn));
			if (thr == NULL)
				printf("ERROR THREAD");

			concurrent_counter_t *dynamic_counter = malloc(sizeof(concurrent_counter_t));
			assert(dynamic_counter != NULL);

			init(dynamic_counter);

			for(int l = 0 ; l < j; l++)
			{
				thr[l].counter = dynamic_counter;
				thr[l].cpu_nr = l % i;
				pthread_create(&thr[l].thread, NULL, &job, &thr[l]);
			}


			for (int k = 0; k < j; k++)
			{
				pthread_join(thr[k].thread, NULL);
			}
			printf("dynamic counter = %d\n", get(dynamic_counter));
			free(dynamic_counter);
			free(thr);
		}
	}
	printids("main -> ");

	exit(0);
}

