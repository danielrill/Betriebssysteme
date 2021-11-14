// Homework 19
// Tlb.c
// 12.11.2021
// Daniel Rill, Sebastian Kaeser
// Gruppe 11 Betriebssysteme WS2021

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/fcntl.h>
#include <stdint.h>
#include <sched.h>
#include <inttypes.h>


void help(){
        printf("Welcome to tlb.c\n");
        printf("The following flags are supported\n");
        printf("-n\tset the number of pages\n");
        printf("-l\tset the number of trials (to acess a page) \n");
        printf("-h\tfor help\n");
        printf("Dial the number 0123456 for support\n");

}


//---------------------------------------------MAIN-----------------------
int main(int argc, char** argv){

        //cpu_set_t my_set;              // The cpu_set_t represents a set of CPUs.
        //CPU_ZERO(&my_set);         // Clears set, so that it contains no CPU's
        //CPU_SET(0, &my_set);       // Add CPU cpu to set
        //sched_setaffinity(0,sizeof(cpu_set_t), &my_set);
        //


        setpriority(PRIO_PROCESS, 0, -20);     
        nice(-20);                           

        uint32_t numPages;
        uint32_t numTrials;
        long pagesize = sysconf(_SC_PAGESIZE);
	struct timespec start_time,end_time;	
        int optval;

	while((optval = getopt(argc,argv,"n:l:h")) != -1){
		switch(optval){
			case 'n':
                                numPages = atoi(optarg);
				break;
			case 'l':
                                numTrials = atoi(optarg);
				break;
                        case 'h':
                                help();
                                return 0;
			default:
				printf("Incorrect option\n");
				return 1;

		}

	}

        printf("Folgende Parameter wurden gewählt\n");
        printf("NumTrials="  "%" PRId32 "\n",numTrials);
        printf("NumPages="  "%" PRId32 "\n",numPages);

        int *arr = (int*)malloc((size_t) numPages * (size_t)pagesize);
        if(arr == NULL){
                fprintf(stderr,"Fehler bei Speicherreservierung!\n");
                return 1;
        }
        int jump = pagesize / sizeof(uint32_t); 
        if(clock_gettime(CLOCK_MONOTONIC_RAW,&start_time) < 0){
                fprintf(stderr,"Erste Zeitmessung fehlgeschlagen!\n");
                return 2;
        }
        
        for(uint32_t j=0; j < numTrials; j++){
                for(uint32_t i=0; i < numPages * jump; i+= jump){
                        arr[i] += 1;
                }
        }
        
        if(clock_gettime(CLOCK_MONOTONIC_RAW,&end_time) < 0){
                fprintf(stderr,"Zweite Zeitmessung fehlgeschlagen!\n");
                return 2;
        }


        end_time.tv_sec -= start_time.tv_sec;
        end_time.tv_nsec -= start_time.tv_nsec;

        

        printf("\nProgrammauswerung: \n");
        printf("[NumPages = " "%" PRId32 "]\n",numPages);
        printf("[NumTrials = "  "%" PRId32 "]\n",numTrials);
        printf("[Nanosekunden: %lu]\n",end_time.tv_nsec/numTrials/jump);
        
        return 1;
}




