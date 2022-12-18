// Christopher Cao
// ch282858
// COP4600 - Fall 2022

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *data = (int *)malloc(100);
	int temp;
	data[100] = 1453;
	
	free(data);
	temp = data[0];
	printf("%d\n", temp);
	
	return 0;
}
