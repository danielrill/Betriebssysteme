#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

int main()
{
    char buf[BUFSIZ];
    int pipefd[2];
    if (pipe(pipefd) == -1)
        exit(1);

    int rc = fork();
    assert(rc >= 0);
    
    if (rc == 0)        //child 1
    {
        close(pipefd[0]);
        write(pipefd[1], "Helloo", strlen("Helloo"));
        close(pipefd[1]);
        printf("Hello from Child NR1 (ppid:%d) , (pid:%d) \n", getppid(), getpid());
        
    } else { //child2
    
        int rc2 = fork();
       assert(rc2 >= 0);
       
       if (rc2 == 0)
       {
            close(pipefd[1]);
            
            while(read(pipefd[0], &buf, 1) > 0) {
                write(STDOUT_FILENO, &buf, 1);
            }
            write(STDOUT_FILENO, "\n",  1);
            
            printf("Hello from Child NR2 (ppid:%d) , (pid:%d) \n", getppid(), getpid());
       } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(-1, NULL, 0);
            waitpid(rc2,NULL, 0);
            printf("RC1 : %d, RC2 : %d \n", rc, rc2);
            printf("Hello from Parent (ppid:%d) , (pid:%d) \n", getppid(), getpid());
       }
    }

    return 0;
}


/*

1 variante ###########
a->b
        b+
            b->c
                    c+
                    
                    
2 variante ##########
a->b
        b+
a->c
        c+
        
*/
