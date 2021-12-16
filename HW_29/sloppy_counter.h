#include <pthread.h>
#include <unistd.h>

#ifndef SLOPPY_COUNTER_H
#define SLOPPY_COUNTER_H

//#define NUMCPUS sysconf(_SC_NPROCESSORS_ONLN)

#define NUMCPUS 4


typedef struct sloppy_counter_t {
    int                             global_count;
    pthread_mutex_t     global_lock;
    int                             local[NUMCPUS];
    pthread_mutex_t     local_lock[NUMCPUS];
    int                             threshold;
} sloppy_counter_t;

void init_sloppy(sloppy_counter_t *c, int threshold);

void update(sloppy_counter_t *c, int threadID, int amount);

int get_sloppy(sloppy_counter_t *c);


#endif
