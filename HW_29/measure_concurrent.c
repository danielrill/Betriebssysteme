
/* 
measure_concurrent.c

Programm zum testen und messen aller Datenstrukturen


*/

#define _GNU_SOURCE
#include <sched.h>

#include "concurrent_counter.h"
#include "measure_time.h"
#include "sloppy_counter.h"
#include "linked_list.h"
#include "hand_over_list.h"
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

void *linked_job(void* args) {

    list_t *l = (list_t*) args;
    List_Lookup(l, 0);
    printf("Searching key 0: %d\n", List_Lookup(l,0));
    
    return ((void*) 0);
}

void *hand_job(void *args) {
    hand_list_t *list = (hand_list_t *) args;
    Hand_List_Lookup(list , 101);
    printf("Searching key 0: %d\n", Hand_List_Lookup(list,101));
    
     return ((void*) 0);
}

void time_measure(struct timespec start, struct timespec end) {

    struct timespec ret;
    if (start.tv_nsec > end.tv_nsec ) {
             ret.tv_sec--;
             ret.tv_nsec = end.tv_nsec + 10e9 - start.tv_nsec;
        } else {
             ret.tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        
          ret.tv_nsec = end.tv_nsec - start.tv_nsec;
			printf("took you %ld sec and %ld ns\n" , ret.tv_sec, ret.tv_nsec);
}



void concurrent_measure(int cpu_amount) {

    printf("\n\n \t### Starting Concurrent Counter Measurement ### \n\n");

    struct timespec start, end, ret;
    

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
			assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start) == 0);

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
			assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end) == 0);
			
			time_measure(start, end);

			
			printf("dynamic counter = %d  on CPU : %d  Threads per CPU %d \n", get(dynamic_counter), i, j);
			free(dynamic_counter);
			free(thr);
			
		}
	}
    printf("\n\n \t### End of Concurrent Counter Measurement ### \n\n");
}

void sloppy_measure(int cpu_amount) {
    printf("\n\n \t### Starting Sloppy Counter Measurement ### \n\n");
    struct timespec start_sloppy, end_sloppy, ret_sloppy;
    
    	for (int i= 0; i<= cpu_amount ; i++)
	{
	    double threshold = pow(2.0, (double) i);
		for (int j = 1; j <= cpu_amount; j++)
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
			
			
			printf("\n \t### CPU number : \t%d | of %d ###\n", i, cpu_amount);
			assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start_sloppy) == 0);

			for(int l = 0 ; l < j; l++)
			{

				pthread_create(&threads[l], NULL, &sloppy_job, &sloppy_thread);
			}


			for (int k = 0; k < j; k++)
			{

				pthread_join(threads[k], NULL);
			}
			assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end_sloppy) == 0);
			
			
        time_measure(start_sloppy, end_sloppy);
			
			printf("dynamic counter = %d  on CPU : %d  Threads per CPU %d \n", get_sloppy(dynamic_counter), i, j);
			free(threads);
			free(dynamic_counter);
			
		}
	}
    
    printf("\n\n \t### End of Sloppy Counter Measurement ### \n\n");
}

void list_measure() {

    printf("\n\n \t### Start of LINKED LIST Measurement ### \n\n");

    int list_length = 100;
    int thread_amount = 4;
    struct timespec start_list, end_list, ret_list;
    
    list_t *list = malloc(sizeof(list_t));
    assert(list != NULL);
    
    List_Init(list);
    
    for(int i = 0; i < list_length; i++) {
        List_Insert(list, rand()%100);
    }
    
    for (int i = 1; i<thread_amount; i++) {
        pthread_t *threads = malloc((size_t)i * sizeof(pthread_t));
        assert(threads != NULL);
        
       assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start_list) == 0);
        for (int j = 0; j < i; j++)
            pthread_create(&threads[j], NULL, &linked_job, list);
    
       for (int k = 0; k < i; k++)
            pthread_join(threads[k], NULL);
            
            
        // time ############################################
            
        assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end_list) == 0);
        printf("(WORST-CASE)");
        time_measure(start_list, end_list);
            
        free(threads);
        printf("LinkedList , Threads per CPU %d \n", i);
    }
    
    free(list);
    
    printf("\n\n \t### End of LINKED LIST Measurement ### \n\n");
}

void hand_list_measure() {

    printf("\n\n \t### Start of HAND OVER LIST Measurement ### \n\n");
    
    int hand_list_length = 100;
    int hand_thread_amount = 4;
    struct timespec start_hand, end_hand, ret_hand;
    
    hand_list_t *hand_list = malloc(sizeof(hand_list_t));
    assert(hand_list != NULL);
    

    
    Hand_List_Init(hand_list, 10 );

    for(int i = 0; i < hand_list_length; i++) {
        Hand_List_Insert(hand_list, rand()%100);
    }
    
    for (int i = 1; i<hand_thread_amount; i++) {
        pthread_t *threads = malloc((size_t)i * sizeof(pthread_t));
        assert(threads != NULL);
        
       assert (clock_gettime(CLOCK_MONOTONIC_RAW, &start_hand) == 0);
        for (int j = 0; j < i; j++)
            pthread_create(&threads[j], NULL, &hand_job, hand_list);
    
       for (int k = 0; k < i; k++)
            pthread_join(threads[k], NULL);
            
            
        // time ############################################
            
        assert (clock_gettime(CLOCK_MONOTONIC_RAW, &end_hand) == 0);
        printf("(WORST-CASE)");
        time_measure(start_hand, end_hand);
    }
    
    free(hand_list);    
    
    printf("\n\n \t### End of HAND OVER LIST Measurement ### \n\n");
}


int main(){

    int cpus = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if (cpus == -1)
		printf("Error no CPU detected\n");
		
    printf(" \t### Number of CPU's detected %d ###\n", cpus);
    
    concurrent_measure(cpus);
    printf(" #####################################################");
    sloppy_measure(cpus);
    printf(" #####################################################");
    list_measure();
    printf(" #####################################################");
    hand_list_measure();
   
	exit(0);
}

