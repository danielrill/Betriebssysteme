#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // POSIX

int main(int argc, char** argv) {
	printf("hello world (pid:%d) \n", (int) getpid());
	int rc = fork();

	if(rc < 0) {
		// fork failed

		fprintf(stderr, "fork failed \n");
		exit(1);

	} else if (rc == 0) {
		// child (new process)
		printf("Hello, i am a child (pid:%d)\n", (int) getpid());
	} else {
		// parent goes down this path (main)
		printf("Hello, i am a parent of %d (pid:%d) \n", rc, (int) getpid());
	}
	return 0;
}
