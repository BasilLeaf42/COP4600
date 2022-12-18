// Christopher Cao
// ch282858
// COP4600 - Fall 2022

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *integer = (int *)malloc(sizeof(int));
	*integer = 0;
	
	printf("%d\n", *integer);
	
	return 0;
}
