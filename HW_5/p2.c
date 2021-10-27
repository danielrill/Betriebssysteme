#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // POSIX
#include <sys/wait.h>

int main(int argc, char** argv) {
	printf("hello world (pid:%d) \n", (int) getpid());
	//int x = 100;
	int rc = fork();
	
	if(rc < 0) {
		// fork failed
		
		fprintf(stderr, "fork failed \n");
		exit(1);
		
	} else if (rc == 0) {
		// child
		//printf("_CHILD_ x am anfang %d\n", x);
		printf("Hello, i am a child (pid:%d)\n", (int) getpid());
		//int x = 102;
		//printf("Wert von x : %d\n", x);
	} else {

		int rc_wait = wait(NULL);	// parent process will wait until the child has been executed
		printf("\nHello, i am a parent of %d (rc_wait:%d) (:%d) \n", rc ,rc_wait ,(int) getpid());
		//printf("_PARENT_ x am anfang %d\n", x);
		//int x = 104;
		//printf("Wert von x : %d\n", x);
	}
	return 0;
}

