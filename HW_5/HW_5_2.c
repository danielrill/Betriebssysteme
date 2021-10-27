#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


int main()
{
    FILE *f = fopen("file.txt", "w+");
    int rc = fork();
	if (rc < 0)
	{
		fprintf(stderr, "Fehler fork(), (errno: %d)\n", errno);
		exit(1);
	}
	if (rc == 0)    // child
	{
	    //setpriority(-10);                     // not reliable !
	    fwrite("im a child \n", 1, 13, f);
	    fclose(f);
	    
	    exit(1);
	} else {
	    //wait(NULL)
	    fwrite("im the parent \n", 1, 16, f);
	    fclose(f);
	    exit(1);
	}
	return 0;
}
