#include <stdlib.h>
#include <stdio.h>

#define SIZE 100

int main()
{
	int *data = (int*) malloc(sizeof(SIZE));
	
	//data[2] = 100;
	//data[50] = 777;
	data[SIZE] = 0;
	


	printf("value of int data: %d; address %p \n",  *data,(void *) &data); 
	//printf("value of int data[2]: %d; address %p \n", data[2], (void *) &data[2]); 
	//printf("value of int data[50]: %d; address %p \n", data[50], (void *) &data[50]); 
	
	free(data);
	return 0;
}
