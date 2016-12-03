#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char *p = NULL;

	p = malloc(10*sizeof(char));

	strcpy(p,"012345678912");

	printf("%s\n",p);
	return 0;
}
