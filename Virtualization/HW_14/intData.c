#include <stdlib.h>
#include <stdio.h>

#define SIZE 100



int main()
{
	int *data = (int*) malloc(sizeof(SIZE));
	
	int counter = 0;
	
	for( int i = 0; i < SIZE; i++)
	{
	   counter ++;	
	    data[i] = rand() % 20;
	     int a = data[i];
		 printf(" %d,",a);
		 
		 
		 if(counter % 10 == 0)
				printf("\n");
	
	}


	free(&data[50]);


	printf("value of int data: %d; address %p \n",  *data,(void *) &data); 
	printf("value of int data[2]: %d; address %p \n", data[2], (void *) &data[2]); 
	printf("value of int data[50]: %d; address %p \n", data[50], (void *) &data[50]); 
	
	
	return 0;
}
