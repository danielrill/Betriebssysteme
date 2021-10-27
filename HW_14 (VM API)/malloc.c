#include <stdio.h>
#include <stdlib.h>

int main()
{
   printf("Welcome to lost bytes ! \n");
	int *eger = (int *) malloc(sizeof(int));
	printf("before setting the ptr = Null\n");
	
	*eger = 777;    

	printf("address of int eger: %p \n", (void *) &eger); 
	printf("value of int eger: %d \n", *eger);


    return 0;
}
