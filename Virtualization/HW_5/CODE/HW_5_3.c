#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
//#include <sys/time.h>
//#include <sys/resource.h>

int main()
{
    int done = 0;
	int rc = fork();
	if (rc < 0)
	{
		fprintf(stderr, "Fehler fork(), (errno: %d) \n", errno);
	}
	else if ( rc == 0 )	// child
	{

		//nice(-20);
		printf("Hello, from Child\n");
	    //done = 1;                                               
	   

	}
	else
	{
	    //if (done) {
	    //setpriority(PRIO_PROCESS, 0, 20);     // klappt ~ 1 von 10 mal
	    //nice(20);                                              // klappt ~ 1 von 10 mal
	    
       sleep(1);                                               // klappt auch immer ? (Achtung nicht 100% !!)
       // alle drei oben "cheap and dirty !!"
       // nicht mit Zeit synchronisieren !!! direkt Kommunizieren !
       // Pipe eignet sich sehr gut // oder klar wait()
		printf("Goodbye from your Father\n");
		exit(0);
		//}
	}

	return 0;
}
