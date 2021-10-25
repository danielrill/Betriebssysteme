#include <stdlib.h>
#include <pthread.h>

// Using two Mutexes to protect a Data_structure
/*
    Since the new Structure is placed on a global list, other threads can find it,
       so we need to block them if they try to access the new structure, until we are done with init.
       
     Find; locks the hash list lokc and searches for the requested structure
                an either increase reference count and return pointer to the struct (found) 
                
                Lock ordering: first we lock the hashlist lock in foo_find before foo_hold locks the foo structures f_lock mutex
                
       Release; because of two_mutexes now more complicated.
                            If last reference -> unlock structure mutex (to aquire the hashlist lock) since we need to remove from the hashlist
                                    then we reaquire the structure mutex.
                           Because we could have blocked since the last time we held the structure mutex, we need to recheck the condition (free it ?)
                                 if another thread found the structure and added a reference, we simply need to decrement the reference count, unlock and return.
                                 
        But still, this approach can be simpliefied considerably by using the hash list lock to protect the structure reference count, too
*/

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int                             f_count;
	pthread_mutex_t     f_lock;
	int                             f_id;
	struct foo                  *f_next;        // Protected by hashlock
	// possibly more stuff here
};

struct foo* foo_alloc(int id)
{
	struct foo *fp;
	int idx;

	if ((fp = malloc(sizeof(struct foo)) != NULL)
	{
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init( &fp->f_lock, NULL) != 0)
		{
			free(fp);
			return(NULL);
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		// Continue init
		pthread_mutex_unlock(&fp->f_lock);
	}
	return(fp);
}

void foo_hold(struct foo *fp)
{
	// Add a reference to the object
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;							// Critical Section
	pthread_mutex_unlock(&fp->f_lock);
}

void foo_find(struct foo *fp)
{
    // FInd an existing Object
    struct foo *fp;
    
    pthread_mutex_lock(&hashlock);                              // Critical Section
    
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next)
    {
        if (fp->f_id == id)
        {
            foo_hold(fp);
            break;
        }
    }
    
    pthread_mutex_unlock(&hashlock);
    return(fp);
}

void foo_release(struct foo *fp)
{
	// Release a reference to the object
	struct foo *tfp;
	int idx;
	
	pthread_mutex_lock(&fp->f_lock);
	if (fp->f_count++ == 1)							// Critical Section (last reference)
	{
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);
		// Recheck the condition !
		if (fp->f_count != 1)
		{
		    fp->f_count--;
		    pthread_mutex_unlock(&fp->f_lock);
		    pthread_mutex_unlock(&hashlock);
		    return;
		}
		// remove from List
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp)
		{
		    fh[idx] = fp->f_next;
		} else {
		    while (tfp->f_next != fp)
		    {
		        tfp = tfp->f_next;    
		    }
		    tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);		
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
	    fp->f_count--;
		pthread_mutex_unlock(&fp->f_lock);
	}
}
