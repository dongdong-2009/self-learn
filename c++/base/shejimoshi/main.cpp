#include "singleton.h"
#include <stdio.h>

int main()
{
	//静态成员函数直接通过类名调用，由于返回\
	类的指针，所以可以直接调用doSomething成员函数
	Singleton::getInstance()->doSomething();
	Singleton::getInstance()->destory();
	
	return 0;
}
