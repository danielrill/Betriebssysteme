#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char **argv, char **envp)
{
    //char **envp = {}
  
    int rc = fork();
     if (rc < 0)
	{
		fprintf(stderr, "Fehler fork(), (errno: %d) \n", errno);
	}
	
	if (rc == 0) 
	{
        if(atoi(argv[1]) == 1)              // execl
        {
	        printf("####################\n");
	        execl("/bin/ls", "/bin/ls", "-alt", NULL);
	        exit(1);
	    }
	 
	    if(atoi(argv[1])== 2)               // execv
	    {
	        char* args_[] = {"ls", "-alt", NULL};
	        printf("####################\n");
	        execv("/bin/ls", args_);	    
	        exit(1);
	    }
	    if(atoi(argv[1])== 3)               // execle
	    {
	        char *envp_[] = {"Test-Environment", NULL}; 
	        printf("####################\n");
	        execle("/bin/ls", "ls", "-alt", (char*) NULL, envp_);	    
	        exit(1);
	    }
        if(atoi(argv[1])== 4)               // execve
	    {
	        char* args_[] = {"/bin/ls", "-alt", NULL};
	        char *envp_[] = {"Test-Environment", NULL}; 
	        printf("####################\n");
	        execve("/bin/ls", args_, envp);	    
	        exit(1);
	    }
	    
	    if(atoi(argv[1]) == 5)          // execvp
	    {
	        char* args_[] = {"ls", "-alt", NULL};
	        printf("####################\n");
	        execvp("ls", args_);
	        exit(1);	
	    }
	    
	    if(atoi(argv[1]) == 6)          // execlp
	    {
	        printf("####################\n");
	        execlp("ls", "ls", "-alth", (char*)NULL);
	        exit(1);	
	    }
	    
         if(atoi(argv[1]) == 7)          // fexecve
         {
            int fd = 0;
	        char* args_[] = {"/bin/ls", "-alt", NULL};
	        char *envp_[] = {"Test-Environment", NULL}; 
            if ((fd = open("/bin/ls", O_RDONLY)) == -1)
                exit(1);
          fexecve(fd, args_, envp_);
         }
	    
	} else {
	
	    printf("Hello from parent (pid: %d )\n", getpid());
	    exit(1);
	}
          
	return 0;
}
