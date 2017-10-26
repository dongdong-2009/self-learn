#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	short *p = 0;
	int *q = 0;
	char **o = 0;
	printf("[short*++]%d\n[int*++]%d\n[char**++]%d\n",++p,++q,++o);
	return 0;
}
