#include <pthread.h>

/*
	The condition represenst the state of the work queue
	We protect the condition with a mutex and evaluate the condition in a _WHILE_LOOP_

	When we put a message on the work queue, we need to hold the mutex, but we dont need to
	hold the mutex when we signal the waiting threads.
	As long as it is okay for a thread to pull a message off the queue before we call signal(),
	we can do this after releasing the mutex.

	Since we check the condition in a while loop, this doesnt present a problem;
	a thread will wakpe up, find that the queue is still empty, and go back to waiting again!

	(If the code couldnt tolerate this race, we would need to hold the mutex when we signal the threads)
*/

struct msg{
	struct msg *m_next;
	// more stuff here
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg(void)
{
	struct msg *mp;

	for(;;) {
		pthread_mutex_lock(&qlock);
		while(workq == NULL)					// Always prefer while !
			pthread_cond_wait(&qready, &qlock);
		mp = workq;
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		// now process the msg mp
	}
}

void enqueue_msg(struct msg *mp)
{
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);
}
