#include "singleton.h"

#include <iostream>
using namespace std;

//类的静态成员变量一定要在类的外面进行初始化
Singleton* Singleton::instance = NULL;

Singleton::Singleton()
{
	cout<<"Singleton instance"<<endl;
}

Singleton::~Singleton()
{
	
}

Singleton* Singleton::getInstance()
{
	Singleton *ret = instance;
	if(ret == NULL)
	{
		//new的时候通过类的公有函数getInstance访问类的私有构造函数，由于构造函数私有了，所以不能创建对象,因为创建对象会调用类的私有构造函数
		instance = new Singleton();//调用不带参数的构造函数
		ret = instance;
	}

	return ret;
}

void Singleton::doSomething()
{
	cout<<__func__<<","<<__LINE__<<endl;
}

void Singleton::destory()
{
	cout<<"destory instance"<<endl;
	delete this;
	instance = NULL;
}
