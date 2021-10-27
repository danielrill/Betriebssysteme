#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
    pid_t       ppid;
    pid_t       cpid;
    int             status;
    
    
	int rc = fork();
	if ( rc < 0 )
	{
		fprintf( stderr, "Fork failed" );
		exit(1);
	}
	
	
	else if ( rc == 0 )     // child
	{
	        // wait(): on success, returns the process ID of the terminated child; on error, -1 is returned.
          cpid = wait(NULL);                                // -1 , nothing to wait for 
          printf("child waits for: %d\n", cpid);
          printf("child pid: %d\n\n", getpid());
	}
	else
	{
       ppid = wait(NULL);                                   // child process it is waiting for
       printf("Parent waits for: %d \n", ppid);
       printf("parent pid: %d\n\n", getpid());
        
	}

	return 0;
}
