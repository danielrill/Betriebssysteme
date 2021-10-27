#include <stdlib.h>
#include <stdio.h>

#define SIZE 100

int main()
{
	int *data = (int*) malloc(sizeof(SIZE));
	
	//data[2] = 100;
	//data[50] = 777;
	printf(" arrayptr %p \n", &data);
	
	printf("array[0] %p \n", &data[0]);
	printf(" array[10] %p \n", &data[10]);
	printf(" array[100] %p \n", &data[100]);
	
	printf(" array[200] %p \n", &data[200]);
	
	data[200] = 2;
	printf("%d data[200] \n", data[200]);
	data[SIZE] = 0;
	

              //  [[0 ... 99] .. [200]]

	printf("value of int data: %d; address %p \n",  *data,(void *) &data); 
	//printf("value of int data[2]: %d; address %p \n", data[2], (void *) &data[2]); 
	//printf("value of int data[50]: %d; address %p \n", data[50], (void *) &data[50]); 
	
	free(data);
	return 0;
}
