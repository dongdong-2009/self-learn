#include <stdio.h>


void fun(void)
{
	int i;
	for(i = 0;i < 10;i++)
		printf("==================>%d\n",i);
}

int main(void)
{
	printf("===================start\n");
	fun();
	return 0;
}
