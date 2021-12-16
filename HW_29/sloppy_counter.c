#include "sloppy_counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


void init_sloppy(sloppy_counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global_count = 0;
    pthread_mutex_init(&c->global_lock, NULL);
    
    for (int i = 0; i < NUMCPUS; i++)
    {
        c->local[i] = 0;
        pthread_mutex_init(&c->local_lock[i], NULL);
    }
}

void update(sloppy_counter_t *c, int threadID, int amount) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->local_lock[cpu]);
    c->local[cpu] += amount;
    
    if(c->local[cpu] >= c->threshold) {
        //Transfer to global
        pthread_mutex_lock(&c->global_lock);
        c->global_count += c->local[cpu];
        pthread_mutex_unlock(&c->global_lock);
        c->local[cpu] = 0;
    }
    
    pthread_mutex_unlock(&c->local_lock[cpu]);
}

// Return approximate global amount
int get_sloppy(sloppy_counter_t *c) {
    pthread_mutex_lock(&c->global_lock);
    int val = c->global_count;
    pthread_mutex_unlock(&c->global_lock);
    return val;     // approx !
}
