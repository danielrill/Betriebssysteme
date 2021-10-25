#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>


int main( int argc, char ** argv )
{
	int rc = fork();
	if (rc < 0)
	{
		fprintf(stderr, "Fork failed");
		exit(1);
	}
	else if ( rc == 0 )
	{
		// Schließt den Standard-Output Deskriptor
		close(STDOUT_FILENO);
		// Schließt den Standard-Error Deskriptor
		close(STDERR_FILENO);

		//fprintf(stdout, "Test");
		fprintf(stderr, "TEST_2");
		fprintf(stdout, "TEST_3");
		open("./redirect.output", O_CREAT|O_WRONLY);

		char *myargs[3];
		myargs[0] = strdup("wc");
		myargs[1] = strdup("redirect.c");
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
	} else {
		int wc = wait(NULL);
		printf("HELLO, I am the Parent of %d (rc_wait: %d) (pid: %d) \n", rc, wc, (int) getpid());
	}
	return 0;
}
