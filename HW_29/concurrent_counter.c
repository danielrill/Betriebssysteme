#include "concurrent_counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


#ifndef cpu_set_t
#define cpu_set_t cpuset_t
#endif


void init(concurrent_counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(concurrent_counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value ++;
    pthread_mutex_unlock(&c->lock);
}

void decrement(concurrent_counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value --;
    pthread_mutex_unlock(&c->lock);
}

int get(concurrent_counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int ret = c->value;
    pthread_mutex_unlock(&c->lock);
    
    return ret;
}
