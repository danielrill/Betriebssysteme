#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


// Add global var
// print address of vars

int main ()
{

	int v1 = 100;
	static short v2  = 3;
	
	int *p;
	p = malloc(sizeof(int));
	if (p < 0)
	    exit(1);

	int rc = fork();
	if ( rc < 0 )
	{
		fprintf( stderr, "Fork failed" );
		exit(1);
	}
	else if ( rc == 0 )
	{
		sleep(2);
		v1 ++;
		v2 ++;
		*p = 10;
		printf( "Child (pid: %d) v1 = %d, v2 = %d, p = %d \n",(int) getpid(), v1, v2, *p );
	}
	else
	{
		v1 +=2;
		v2 +=2;
		*p = 20;
		printf( "Parent (pid: %d) v1 = %d, v2 = %d, p = %d \n",(int) getpid(), v1, v2, *p );
	}

	printf("Goodbye \n");
	return 0;
}
