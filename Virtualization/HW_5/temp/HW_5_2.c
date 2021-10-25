#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
/*
static void write_it(FILE *f, char *msg)
{
	char *ptr_msg;
	int x;
	for (ptr_msg = msg; ( x = *ptr_msg++) != 0;)
	{
		if(fputc(x, f) != x)
			exit(1);
		if(fflush(f) == EOF)
			exit(1);
	}
}
*/

int main()
{
	//FILE *f = fopen("file.txt", "w+");
	int in = open("/home/daniel/Schreibtisch/OSTEP/Virtualization/HW_5/CODE/file.txt", O_WRONLY);
	if (in  < 0)
	{
		fprintf(stderr, "Quelle konnte nicht geöffnet werden!, (errno: %d)\n", errno);
	}

	int rc = fork();
	if (rc < 0)
	{
		fprintf(stderr, "Fehler fork(), (errno: %d)\n", errno);
		exit(1);
	}
	if (rc == 0)
	{
	    
		write(in, "Hello from child",17);
		//write_it(f, "Hello from child!\n");
	}
	else
	{
	    write(in, "Hello from Parent",18);
		//write_it(f, "Goodbye from Parent!\n");
	}
	close(in);
	return 0;
}
