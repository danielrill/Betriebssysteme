#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Welcome to a segmentation fault ! \n");
	int *eger = (int *) malloc(sizeof(int));
	printf("before setting the ptr = Null\n");
	
	eger = NULL;    

	printf("address of int eger: %p \n", (void *) &eger); 
	printf("value of int eger: %d \n", *eger); // thats the bad one
	free(eger);

	return 0;

}
/*
 Causes a  -> segmentation fault (core dumped) , when  we access the variable
*/
