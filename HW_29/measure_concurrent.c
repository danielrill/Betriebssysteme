#define _GNU_SOURCE
#include <sched.h>

#include "concurrent_counter.h"
#include "measure_time.h"
#include "sloppy_counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#define MAGIC 100

typedef struct thr_concurrent {
	concurrent_counter_t *counter;
	pthread_t thread;

	int cpu_nr;
} thr_concurrent;

typedef struct thr_sloppy {
    sloppy_counter_t    *counter;
    int threshold;
    int amount;
    int threads;
} thr_sloppy;

void printids(const char *s) {
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();

	printf("( %s ) pid ( %lu ) | tid ( %lu )\n", s, (unsigned long) pid, (unsigned long) tid);
}


void *job(void *args) {
	//printids("worker ->");
	thr_concurrent *tmp = (thr_concurrent*) args;
	cpu_set_t cpuset;
	CPU_SET(tmp -> cpu_nr, &cpuset);
	pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    printids("worker ->");

	for( int i = 0; i < MAGIC; i++)
	{
		increment(tmp-> counter);
	}

	return ((void*) 0);
}

void *sloppy_job(void *args) {
    thr_sloppy *tmp = (thr_sloppy*) args;
    pthread_t threadID = pthread_self();
    printids("sloppy_worker ->");
    
    for(int i = 0; i < MAGIC; i++)
    {
        update(tmp->counter, (int)threadID, tmp->amount);
    }
    return ((void*) 0);
}



void concurrent_measure(int cpu_amount) {

    printf("\n\n \t### Starting Concurrent Counter Measurement ### \n\n");
        //struct timespec results[2];
    //struct timespec start, end, ret;
	

	for (int i= 1; i<=cpu_amount; i++)
	{
		for (int j = 1; j <= cpu_amount; j++)
		{
			thr_concurrent *thr = malloc((size_t)j * sizeof(thr_concurrent));
			if (thr == NULL)
				printf("ERROR THREAD");

			concurrent_counter_t *dynamic_counter = malloc(sizeof(concurrent_counter_t));
			assert(dynamic_counter != NULL);

			init(dynamic_counter);
			
			printf("\n \t### CPU number : \t%d | of %d ###\n", i, cpu_amount);
			//assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start) == 0);
           //measure(main, 2, results);
			for(int l = 0 ; l < j; l++)
			{
			    //printf("\n ### CPU count :\t %d ###\n", l);
				thr[l].counter = dynamic_counter;
				thr[l].cpu_nr = l % i;
				pthread_create(&thr[l].thread, NULL, &job, &thr[l]);
			}


			for (int k = 0; k < j; k++)
			{
			    //printf("\n ### CPU count :\t %d ###\n", k);
				pthread_join(thr[k].thread, NULL);
			}
			//assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end) == 0);
			
           // ret.tv_nsec = end.tv_nsec - start.tv_nsec;
			//printf("%ld time\n" , ret.tv_nsec);
			
			printf("dynamic counter = %d  on CPU : %d  Threads per CPU %d \n", get(dynamic_counter), i, j);
			free(dynamic_counter);
			free(thr);
			
		}
	}
    printf("\n\n \t### End of Concurrent Counter Measurement ### \n\n");
}

void sloppy_measure(int cpu_amount) {
    printf("\n\n \t### Starting Sloppy Counter Measurement ### \n\n");
    
    	for (int i= 0; i<= NUMCPUS ; i++)
	{
	    double threshold = pow(2.0, (double) i);
		for (int j = 1; j <= NUMCPUS; j++)
		{
		    sloppy_counter_t *dynamic_counter = malloc(sizeof(sloppy_counter_t));
			assert(dynamic_counter != NULL);
			init_sloppy(dynamic_counter, (int) threshold);
			
			pthread_t *threads = malloc((size_t)j * sizeof(pthread_t));
			if (threads == NULL)
				printf("ERROR THREAD");

			thr_sloppy sloppy_thread;
			sloppy_thread.counter = dynamic_counter;
			sloppy_thread.threshold = (int)threshold;
			sloppy_thread.amount = 1;
			sloppy_thread.threads = j;
			
			
			printf("\n \t### CPU number : \t%d | of %d ###\n", i, NUMCPUS);
			//assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start) == 0);
           //measure(main, 2, results);
			for(int l = 0 ; l < j; l++)
			{
			    //printf("\n ### CPU count :\t %d ###\n", l);
				pthread_create(&threads[l], NULL, &sloppy_job, &sloppy_thread);
			}


			for (int k = 0; k < j; k++)
			{
			    //printf("\n ### CPU count :\t %d ###\n", k);
				pthread_join(threads[k], NULL);
			}
			//assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end) == 0);
			
           // ret.tv_nsec = end.tv_nsec - start.tv_nsec;
			//printf("%ld time\n" , ret.tv_nsec);
			
			printf("dynamic counter = %d  on CPU : %d  Threads per CPU %d \n", get_sloppy(dynamic_counter), i, j);
			free(threads);
			free(dynamic_counter);
			
		}
	}
    
    printf("\n\n \t### End of Sloppy Counter Measurement ### \n\n");
}


int main(){

    int cpus = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if (cpus == -1)
		printf("Error no CPU detected\n");
		
    printf(" \t### Number of CPU's detected %d ###\n", cpus);
    
    concurrent_measure(cpus);
    printf(" #####################################################");
    sloppy_measure(cpus);

	exit(0);
}

