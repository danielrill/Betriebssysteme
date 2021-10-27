#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>

#define TOBYTES 1000000


int main(int argc, char** argv){

    printf("sizeof int = %ld\n", sizeof(int));
    printf("PID = %d\n", getpid());
    sleep(4);

	int optval;
	int mbytes;	
	struct timespec time;
	


	while((optval = getopt(argc,argv,"b:t:")) != -1){
		switch(optval){
			case 'b':
				mbytes = atoi(optarg);
				break;
			case 't':
				time.tv_sec = atoi(optarg);
				break;
			default:
				printf("Incorrect option\n");
				return 1;

		}

	}
    printf("\t flag time: %lu \n", time.tv_sec);

	uint64_t bytes = mbytes * TOBYTES;
	uint64_t counter = 0;


	printf("\t flag Bytes: " "%" PRId64 "\n",bytes); 

	int* arr = malloc(bytes);

	if(arr == NULL){
		fprintf(stderr,"Fehler bei Speicherreservierung\n");
		return 2;
	}
	
	struct timespec start_time,end_time;	
    struct timespec elapsed_time;
    
    
	if(clock_gettime(CLOCK_MONOTONIC,&start_time) < 0){
		fprintf(stderr,"Erste Zeitmessung fehlgeschlagen!\n"); 
		return 3;
	}

    printf("\t starttime: %lu \n", start_time.tv_sec);
	do {
		for(uint64_t j=0; j < bytes / sizeof(uint32_t); j++){
			counter ++;	
			arr[j] = rand() % 10;
			uint64_t a = arr[j];
			//printf("%lu",a);
			if(counter % 120 == 0){
				//printf("\n");
			}
		}

		if(clock_gettime(CLOCK_MONOTONIC,&end_time) < 0){
			fprintf(stderr,"Erste Zeitmessung fehlgeschlagen!\n"); 
			return 3;
		}	
        //printf("\t endtime: %lu \n", end_time.tv_sec);
		elapsed_time.tv_sec = end_time.tv_sec - start_time.tv_sec;
	} while( elapsed_time.tv_sec <= time.tv_sec);

	free(arr);

    printf("\t elapsedtime : %lu \n", elapsed_time.tv_sec);
	printf("\tFERTIG\n");


	return 0;
}

