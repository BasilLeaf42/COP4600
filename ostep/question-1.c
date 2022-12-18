// Christopher Cao
// ch282858
// COP4600 - Fall 2022

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *integer = (int *)calloc(1, sizeof(int));
	integer = NULL;
	
	printf("%d\n", *integer);
	free(integer);
	
	return 0;
}
