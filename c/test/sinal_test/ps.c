#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	printf("============%d\n",(system("ps -aux | grep clo | grep -v grep")));
	return 0;
}
