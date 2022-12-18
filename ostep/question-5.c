// Christopher Cao
// ch282858
// COP4600 - Fall 2022

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *data = (int *)malloc(100);
	data[100] = 0;
	
	free(data);
	
	return 0;
}
