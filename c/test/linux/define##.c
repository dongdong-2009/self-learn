#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	##被称为连接符（concatenator），用来将两个Token连接为一个Token。
	注意这里连接的对象是Token就行，而不一定 是宏的变量。
	比如你要做一个菜单项命令名和函数指针组成的结构体的数组，并且希望在函数名和菜单项命令名之间有直观的、名字上的关系。
*/


#include<stdio.h>

#define f(a,b) a##b
#define g(a) #a
#define h(a) g(a)

int main_0()
{
	printf("%s\n",h(f(1,2)));
	printf("%s\n",g(f(1,2)));

	return 0;
}

#define abcdef "hello world"
#define Conn(x,y) x##y

int main()
{
	int n = Conn(123,456);
	char* str = Conn(abc,def);
	
	printf("\n\n%d\n\n",n);
	printf("\n\n%s\n\n",str);
	return 0;
}
