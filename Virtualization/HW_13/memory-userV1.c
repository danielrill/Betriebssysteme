#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Mindestens 2 übergabe Parameter");
		exit(0);
	}
	// Specify: Amount of memory and amount of time
	// Param argv[1] mem
	// Param argv[2] time
	char *endptr;
	int64_t vodoo = 512;
	int64_t mem = strtoll(argv[1], &endptr, 0) * vodoo;
	int64_t time = strtoll(argv[2], &endptr, 0);

	printf("mem :" "%"PRId64 "\n", mem);
	printf("time :"" %"PRId64 "\n", time);

	int64_t *array = (int64_t *) malloc(vodoo * sizeof(int64_t*));
	if (array == NULL)
	{
		fprintf(stderr, "Fehler Speicherallokierung");
		exit(0);
	}

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIX, &start);



	free(array);

	return 0;
}
