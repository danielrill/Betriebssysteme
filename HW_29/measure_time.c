#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "measure_time.h"

int get_diff(struct timespec *start, struct timespec *end, struct timespec *ret) {
    unsigned long start_nsec = start->tv_nsec;
    unsigned long end_nsec = end->tv_nsec;
    
    unsigned long start_sec = start->tv_sec;
    unsigned long end_sec = end->tv_sec;
    
    ret -> tv_sec = end_sec - start_sec;
    ret -> tv_nsec = 0;
    
    if (start_nsec > end_nsec) {
        ret->tv_sec--;
        ret->tv_nsec = end_nsec + 10e9 - start_nsec;
    } else {
        ret->tv_nsec = end_nsec - start_nsec;
    }
    
    return 0;
}


