#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // POSIX
#include <string.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
	printf("hello world (pid:%d) \n", (int) getpid());
	int rc = fork();
	
	if(rc < 0) {
		// fork failed
		
		fprintf(stderr, "fork failed \n");
		exit(1);
		
	} else if (rc == 0) { // Child (new process)
		// child
		printf("Hello, i am a child (pid:%d)\n", (int) getpid());
		char *myargs[3];
		myargs[0] = strdup("wc");
		myargs[1] = strdup("p3.c");
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
		printf("this shouldn't print out");
	} else {
		// parent goes down this path (main)
		int rc_wait = wait(NULL);
		printf("Hello, i am a parent of %d (rc_wait:%d) (:%d) \n", rc ,rc_wait ,(int) getpid());
	}
	return 0;
}

