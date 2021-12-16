#include <pthread.h>

#ifndef CONCURRENT_COUNTER_H
#define CONCURRENT_COUNTER_H

typedef struct concurrent_counter_t {
    pthread_mutex_t lock;
    int value;

} concurrent_counter_t;

void init(concurrent_counter_t *c);


void increment(concurrent_counter_t *c);

void decrement(concurrent_counter_t *c);

int get(concurrent_counter_t *c);

#endif
