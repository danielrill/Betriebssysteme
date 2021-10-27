

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>   // gettimeofday
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>
#include <time.h>         // clock_gettime
#include <sched.h>      // sched_setaffinity
#include <string.h>
#include <inttypes.h>

// optimiere code !

#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif

static uint64_t get_time(void) {
        struct timespec time;
        
        if(clock_gettime(CLOCK_MONOTONIC, &time) != 0) {
            printf("ERROR\n");
            exit(1);
        }
        return ((uint64_t) time.tv_sec) *1000 * 1000 * 1000 + time.tv_nsec;
}

int main()
{

    // Sys-Call

    int it_value = 100000;
    int fp = open("test.txt", O_WRONLY);

    struct timeval begin, end;
    
    int tsc1 = rdtsc();

    uint64_t sc_start_time = get_time();

    gettimeofday(&begin, NULL);


    for (int i = 1; i < it_value; i++)
    {
        read(fp, NULL, 0);      // ISt schlechter sys-call da I/O ausgeführt wird ( verfälschte zahlen ) 
    }
    // bessere möglichkeit forschleife messen und abziehen 
    // leere forschleife messen mit NOP , eigentliche Zeitmessung davon Abziehen
    // getres()  -> wie Zeiten eingestellt sind 
    // Tipp: alle Fälle abdecken ! (doubles floats vermeiden ! )
    // bei durchschnitten ausreiser vermeiden -> [ range max min ]

    gettimeofday(&end, NULL);
    
    uint64_t sc_end_time = get_time();
    int tsc2 = rdtsc();

    uint64_t elapsed = ( sc_end_time - sc_start_time) / it_value;

    printf("sys-call (gettimeofday) : %ld nanosecs \n", elapsed);

     printf("sys-call (clock_gettime) : %lu nanosecs \n", elapsed);

     printf("sys-call (rdtsc) : %e \n", (double) (tsc2- tsc1) / it_value);

//######################################################

     // Context Switch
     cpu_set_t my_set;              // The cpu_set_t represents a set of CPUs.
     CPU_ZERO(&my_set);         // Clears set, so that it contains no CPU's
     CPU_SET(0, &my_set);       // Add CPU cpu to set
     
     
     int p1[2];
     int p2[2];
     
     if (pipe(p1) == -1 || pipe(p2) == -1)
     {
        fprintf(stderr, "Fehler pipe() \n");
        exit(1);
     }
    
    uint64_t start_time;
    uint64_t end_time;
    
    // keine pipe, es gibt ein sys call der ein sys-call descheduled 
    // Overhead gering halten 
     
     int rc = fork();
     if ( rc < 0 )
     {
        fprintf(stderr, "Fehler fork()");
        exit(1);
     }
     if (rc == 0)
     {
     int value = 10;
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set) == -1) {
            printf("Error,  setaffinity (1) \n");
            exit(1);
        }
        close(p1[0]);
        close(p2[1]);
        printf("Child value : %d \n", value);
        if (read(p2[0], &value, sizeof(value)) == -1)
        {
            fprintf(stderr, "Fehler Read (child)\n");
            exit(1);
        }
        printf("test\n");
         if (write(p1[1], &value, sizeof(value)) == -1)
        {
            fprintf(stderr, "Fehler Write (child)\n");
            exit(1);
        }
   
     } else {
        close(p1[1]);
        close(p2[0]);
        int value_ = 20;
     
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set) == -1) {
            printf("Error,  setaffinity (2) \n");
            exit(1);
        }
         
       start_time = get_time();
       
        if (write(p2[1], &value_, sizeof(value_) ) == -1)
        {
            fprintf(stderr, "Fehler Read (parent)");
            exit(1);
        }
         if (read(p1[0], &value_, sizeof(value_) ) == -1)
        {
            fprintf(stderr, "Fehler Write (parent)");
            exit(1);
        }
       end_time = get_time();
       
        uint64_t elapsed = ( end_time - start_time);
          
        close(p1[0]);
        close(p2[1]);
        wait(NULL);
        
        printf("Parent value : %d\n" ,value_);
        printf("Context-Switch-Time : %lu nanosecs  (%lu microseconds) \n", elapsed, elapsed/1000);
     
     }

    return 0;
}

