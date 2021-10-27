#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


int main(){	

	uint64_t counter = 0;

	while(1){
		
		void* t = malloc(100000);
		if(t == NULL){
			printf("counter: %llu\n",counter*1000000);
			sleep(20);
			return 1;
		}

		counter++;

	}




	return 0;
}


