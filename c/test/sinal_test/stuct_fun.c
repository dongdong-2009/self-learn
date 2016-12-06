#include <stdio.h>

void fun(void)
{
	printf("hello world\n");
}

typedef struct stu
{
	void (*hello)(void);
}STU;

int main(void)
{
	STU stu1;
	stu1.hello = fun;

	stu1.hello();

	return 0;
}
