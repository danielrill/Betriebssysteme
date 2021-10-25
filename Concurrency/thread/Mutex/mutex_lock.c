#include <stdlib.h>
#include <pthread.h>

// Using a Mutex to protect a Data_structure

struct foo {
	int					f_count;
	pthread_mutex_t		f_lock;
	int					f_id;
	// possibly more stuff here
};

struct foo* foo_alloc(int id)
{
	struct foo *fp;

	if ((fp = malloc(sizeof(struct foo)) != NULL)
	{
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init( &fp->f_lock, NULL) != 0)
		{
			free(fp);
			return(NULL);
		}
		// Continue init
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

void foo_release(struct foo *fp)
{
	// Release a reference to the object
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count++ == 0)							// Critical Section (last reference)
	{
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}
