#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

#define	NTHR	8				// Number of threads
#define NUMNUM	800000L		// number of numbers to sort
#define TNUM	(NUMNUM/NTHR)	// number to sort per thread

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

//#define heapsort qsort

//extern int heapsort(void *, size_t, size_t, int(*)(const void*,const void*));

int complong(const void *arg1, const void *arg2)
{
	long l1 = *(long *) arg1;
	long l2 = *(long *) arg2;

	if(l1 == l2)
		return 0;
	else if (l1 < l2)
		return -1;
	else
		return 1;
}

// Worker thread to sort a portion of the set of numbers

void *thr_fn(void *arg)
{
	long		idx = (long)arg;

	qsort(&nums[idx], TNUM, sizeof(long), complong);
	pthread_barrier_wait(&b);

	// go off and perform some more work...

	return((void *) 0);
}

// Merge the results of the individual sorted ranges
void merge()
{
	long		idx[NTHR];
	long		i, minidx, sidx, num;

	for(i = 0; i < NTHR; i++)
		idx[i] = i * TNUM;
	for(sidx = 0; sidx < NUMNUM; sidx++)
	{
		num = LONG_MAX;
		for (i = 0; i < NTHR; i++)
		{
			if ((idx[i] < (i+1) * TNUM) && (nums[idx[i]] < num))
			{
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}

static long unsigned r_next = 1;
int myrand(void)
{
	r_next = r_next * 1103515245 + 1234567;
	return((unsigned)(r_next/65536) % 32768);
}
unsigned int seed = 22;
void mysrand(unsigned int seed)
{
	r_next = seed;
}

int main()
{
	unsigned long 	i;
	struct 			timeval start, end;
	long long 		startusec, endusec;
	double			elapsed;
	int 			err;
	pthread_t		tid;

	// Create initial set of numbers to sort
	mysrand(2);
	for (i = 0; i < NUMNUM; i++)
		nums[i] = myrand();

	//for (i = 0; i < NUMNUM; i++)
	//	printf("%ld, ", nums[i]);

	// Create 8 threads to sort the numbers
	gettimeofday(&start, NULL);
	pthread_barrier_init(&b, NULL, NTHR+1);
	// Note that we count one more thread (the main thread, which also calls merge())
	for (i = 0; i < NTHR; i++)
	{
		err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
		if (err != 0) {
			printf("Error %x \n", errno);
			exit(0);
		}
	}

	pthread_barrier_wait(&b);
	merge();
	gettimeofday(&end,NULL);

	// Print the list
	startusec = start.tv_sec * 1000000 + start.tv_usec;
	endusec = end.tv_sec * 1000000 + end.tv_usec;
	elapsed = (double)(endusec - startusec) / 1000000.0;

	printf("sort took us %.4f seconds\n", elapsed);
	for(i = 0; i < NUMNUM; i++)
		printf("%ld,", snums[i]);

	exit(0);
}

// The performance increase compared to using a single thread is ~ 6 times faster.
