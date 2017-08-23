#include <sys/select.h>
#include <stdio.h>

int main(void)
{
	printf("FD_SETSIZE:%d\n",FD_SETSIZE);
	return 0;
}
