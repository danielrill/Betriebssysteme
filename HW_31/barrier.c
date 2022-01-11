#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

/*
sem_t *Sem_open(char *name, int value) {
  sem_t *sem;
  sem = sem_open(name, O_CREAT, S_IRWXU, value);
  assert(sem != SEM_FAILED);
  return sem;
}

*/

typedef struct __barrier_t {
    // add semaphores and other information here
	sem_t mod_counter;
	sem_t  arrived;
	int num_arrived;
	int num_threads;
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

char *mod_counter = "/mod_counter";
char *arrived= "/arrived";


void barrier_init(barrier_t *b, int num_threads) {
	// set mod_couter to a CV
        sem_init(&b->mod_counter, 0, 1);
	// set arrived to a Lock
        sem_init(&b->arrived, 0, 0);
        b->num_arrived = 0;
        b->num_threads = num_threads;
}

void barrier(barrier_t *b) {
   sem_wait(&b->mod_counter);
   b->num_arrived++;
   if(b->num_arrived == b->num_threads) {
           sem_post(&b->arrived);
   }
   sem_post(&b->mod_counter);
   sem_wait(&b->arrived);
   sem_post(&b->arrived);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    
    /*
    sem_close(b.mod_counter);
    sem_close(b.arrived);
    sem_unlink(mod_counter);
    sem_unlink(arrived);
    */
    
    return 0;
}


