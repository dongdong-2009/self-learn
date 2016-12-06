#include <stdio.h>

typedef struct node{
	void (*ptr)(char *str);
}Node;


void hello(char *str)
{
	printf("hello:%s\n",str);
}

int main(void)
{
	Node n;

	n.ptr = hello;

	n.ptr("mkk");

	return 0;
}
