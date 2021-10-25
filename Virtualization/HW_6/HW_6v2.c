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
int LISTSIZE = 100;




void arithm(uint64_t arr[]){
    uint64_t total = 0;
    for(int i=0; i < LISTSIZE; i++){
       total+=arr[i]; 
    }


    total = total/ LISTSIZE;

    printf("Das arithmetische Mittel bei %d Messungen beträgt:  %lu nsec \n",LISTSIZE,total);
}




int main(int argc, char**argv){

    uint_fast16_t iter = 10000;

    uint64_t *arr = (uint64_t*) malloc(sizeof(uint64_t) * LISTSIZE);

    if(arr == NULL){
            fprintf(stderr,"Fehler bei Speicherallokation\n");
            exit(1);
    }

    for(uint64_t j=0; j < LISTSIZE ; j++){

        struct timespec stime, etime, iterbegin,iterend; 


        clock_gettime(CLOCK_MONOTONIC, &stime);  


        for (uint_fast16_t i=0; i < iter; i++){
            getppid();
        }




        clock_gettime(CLOCK_MONOTONIC, &etime);


        clock_gettime(CLOCK_MONOTONIC,&iterbegin);

        for(uint_fast16_t i=0; i < iter; i++){
        }

        clock_gettime(CLOCK_MONOTONIC,&iterend);





        uint64_t start_time = ((uint64_t) stime.tv_sec) * 1000 * 1000 * 100 + stime.tv_nsec;
        uint64_t end_time = ((uint64_t) etime.tv_sec) * 1000 * 1000 * 100 + etime.tv_nsec;
        uint64_t iterbegin_= ((uint64_t) iterbegin.tv_sec) * 1000 * 1000 * 100 + iterbegin.tv_nsec;
        uint64_t iterend_= ((uint64_t) iterend.tv_sec) * 1000 * 1000 * 100 + iterend.tv_nsec;



        uint64_t elapsed_time = ((end_time - start_time) - (iterend_ - iterbegin_)) / iter;
        if(elapsed_time > 1000){
                LISTSIZE--;
                continue;
        }
        arr[j] = elapsed_time;
        //printf("%llu, ",arr[j]);
        //printf("\n");


   }

    arithm(arr);








    return 0;
}

