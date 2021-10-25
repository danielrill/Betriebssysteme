#define _UNIX03_THREADS 1  

#include <stdio.h>                                                              
#include <stdlib.h>
#include <pthread.h>                                                            
#include <limits.h>                                                            
#include <errno.h>                                                            
                                                                                
int main(void)
{                                                                               
   pthread_attr_t attr;                                                         
   int              rc; 

   void  *mystack;
   size_t mystacksize = 2 * PTHREAD_STACK_MIN;
                                                                                
   if (pthread_attr_init(&attr) == -1) {                                        
      perror("error in pthread_attr_init");                                     
      exit(1);                                                                  
   }                                                                            
                                                                                
   /* Get a big enough stack and align it on 4K boundary. */
   mystack = malloc(PTHREAD_STACK_MIN * 3);
   if (mystack != NULL) {
      printf("Using PTHREAD_STACK_MIN to align stackaddr %p.\n", mystack);
      mystack = (void *)((((long)mystack + (PTHREAD_STACK_MIN - 1)) /
                          PTHREAD_STACK_MIN) * PTHREAD_STACK_MIN);
   } else {
      perror("Unable to acquire storage.");
      exit(2);
   }
                                                                                
   printf("Setting stackaddr to %p\n", mystack);
   printf("Setting stacksize to %lx\n", mystacksize);
   rc = pthread_attr_setstack(&attr, mystack, mystacksize);
   if (rc != 0) {                                           
      printf("pthread_attr_setstack returned: %d\n", rc); 
      printf("Error: %d, \n", errno);
      //printf("Error: %d, /*Errno_Jr: %08x\n", errno, __errno2());
      exit(3);                                                                  
   } else {
      printf("Set stackaddr to %p\n", mystack);
      printf("Set stacksize to %lx\n", mystacksize);
   }

   rc = pthread_attr_destroy(&attr);
   if (rc != 0) {                                     
      perror("error in pthread_attr_destroy");                                  
      printf("Returned: %d, Error: %d\n", rc, errno); 
      //printf("Errno_Jr: %x\n", __errno2());
      exit(4);                                                                  
   }                                                                            

   exit(0);                                                                     
}
