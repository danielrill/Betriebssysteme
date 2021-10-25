#include <stdlib.h>
#include <pthread.h>

/*
	In this example, we lock the queue's reader-writer lock in write_mode whenever we need
	to add a job to the queue or remove a job from the queue.
	Whenever we search the queue, we grab the lock in read mode, allowing all worker threads
	to search the queue concurrently. Using a reader_writer lock will improve performance in this case
	_ONLY_ if threads search the queue more often than they add or remove jobs !

	The worker threads take only those jobs that match their thread ID off the queue.
	Since job structures are used only by one thread at a time, no extra locking is needed.
*/

struct job {
	struct job *j_next;
	struct job *j_prev;

	pthread_t j_id;		// which thread handles this job
};

struct queue {
	struct job		*q_head;
	struct job		*q_tail;

	pthread_rwlock_t q_lock;
};


int queue_init(struct queue *qp)
{
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;

	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0)
	{
		return(err)
	}
	// Continue init
	return(0);
}

void job_insert(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = qp->q_head;
	jp->j_prev = NULL;
	if (qp->q_head != NULL)
		qp->q_head->j_prev = jp;
	else
		qp->q_tail = jp;		// list was empty
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

void job_append(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = NULL;
	jp->j_prev = qp->q_tail;

	if (qp->q_tail != NULL)
		qp->q_tail->j_next = jp;
	else
		qp->q_head = jp;		// list was empty
	qp->q_tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head)
	{
		qp->q_head = jp->j_next;
		if (qp->q_tail == jp)
			qp->q_tail = NULL;
		else
			jp->j_next->j_prev = jp->j_prev;
	} else if(jp == qp->q_tail) {
		qp->q_tail = jp->j_prev;
		jp->j_prev->j_next = jp->j_prev;
	} else {
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}

	pthread_rwlock_unlock(&qp->q_lock);
}

struct job* job_find(struct queue *qp, struct job *jp)
{
	struct job *jp;

	if (pthread_rwlock_rdlock(qp->q_lock) != 0)
		return(NULL);

	for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
		if(pthread_equal(jp->j_id, id))
			break;

	pthread_rwlock_unlock(&qp->q_lock);
	return(jp);
}
