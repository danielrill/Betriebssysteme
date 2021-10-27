#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{

    pid_t       ppid;
    pid_t       cpid;
    pid_t       wait_status;
    
    
	int rc = fork();
	if ( rc < 0 )
	{
		fprintf( stderr, "Fork failed" );
		exit(1);
	}
	
	
	else if ( rc == 0 )     // child
	{
	    // Wait für kleine Anwendungen verwenden, sonst immer waitpid !!
	    cpid = waitpid(-1, NULL, 0);                    // -1 für alle Warten
       printf("child waits for: %d\n", cpid);
       printf("child pid: %d\n\n", getpid());
	}
	else
	{
       ppid = waitpid(rc,  &wait_status, 0);        // ppid; der Prozess auf den gewartet wird
       printf("Parent waits for: %d \n", ppid);
       printf("parent pid: %d\n\n", getpid());
        
	}
	
/*
    Im gegensatz zum herkömmlichen wait() system aufruf,
    ist es mit waitpid möglich das wait-Verhalten mit Argumenten und
    Signalen (2tes Argument) zu beeinflussen;
    
    # 2-tes Argument (wstatus Signale)
    
    WIFEXITED(wstatus)
              returns true if the child terminated normally, that is, by calling exit(3) or
               _exit(2), or by returning
              from main().

       WEXITSTATUS(wstatus)
              returns the exit status of the child.  This consists of the least significant 
              8 bits of the status  argument that the child specified in a call to exit(3) 
              or _exit(2) or as the a rgument for a return statement in main().  
              This macro should be employed only if WIFEXITED returned true.

       WIFSIGNALED(wstatus)
              returns true if the child process was terminated by a signal.

       WTERMSIG(wstatus)
        returns the number of the signal that caused the child process to terminate.  
        This macro should be  em‐
              ployed only if WIFSIGNALED returned true.

       WCOREDUMP(wstatus)
              returns  true  if  the child produced a core dump (see core(5)). 
              This macro should be employed only if
              WIFSIGNALED returned true.

              This macro is not specified in POSIX.1-2001 and is not available on some  
              UNIX  implementations  (e.g.,
              AIX, SunOS).  Therefore, enclose its use inside #ifdef WCOREDUMP ... #endif.

       WIFSTOPPED(wstatus)
              returns  true  if  the  child process was stopped by delivery of a signal; 
              this is possible only if the call was done using WUNTRACED or 
              when the child is being traced (see ptrace(2)).

       WSTOPSIG(wstatus)
              returns the number of the signal which caused the child to stop.  
              This macro should be employed only ifWIFSTOPPED returned true.

       WIFCONTINUED(wstatus)
              (since Linux 2.6.10) returns true if the child process was resumed 
              by delivery of SIGCONT.
    
    # 3- Argument
    < -1      meaning wait for any child process whose process group ID 
                is equal to the absolute value of pid.

       -1     meaning wait for any child process.

       0      meaning  wait  for  any child process whose { process group ID }
               is equal to that of the calling process at
               the time of the call to waitpid().

       > 0    meaning wait for the child whose process ID is equal to the value of pid.

       The value of options is an OR of zero or more of the following constants:

       WNOHANG
              return immediately if no child has exited.
              
              WUNTRACED
              also return if a child has stopped (but not traced via ptrace(2)).  
              Status for  traced  children  which
              have stopped is provided even if this option is not specified.

       WCONTINUED (since Linux 2.6.10)
              also return if a stopped child has been resumed by delivery of SIGCONT.


*/

	return 0;
}
