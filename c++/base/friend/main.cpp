#include <iostream>
using namespace std;

class X;//X的前置声明
class Y
{
	public:
		void f(X*);	//使用指针不会报错
	//	void b(X);	//由于X没有完全声明，所以会报错
	privatei:
		X *pX;
};

#if 0
void Y::b(X a)
{
}
#endif

class X{
	public:
		void initialize();
		friend void g(X *,int);//g是X类的友元函数
		friend void Y::f(X *);//类Y的函数y是X类的友元函数
		friend class Z;			//类Z是X的友元类
		friend void h();		//函数h是类X的友元函数
	private:
		int i;
};

void X::initialize()
{
	this->i = 0;
}

void g(X *x,int i)
{
	//g函数是X类的友元函数可以访问X类的私有成员i
	x->i = i;
}

void Y::f(X *x)
{
	x->i = 47;
}

class Z{
	public:
		void initialize();
		void g(X *x);
	private:
		int j;
};

void Z::initialize()
{
	j = 99;
}

void Z::g(X *x)
{
	x->i += j;
}

void h(){
	X x;
	x.i = 100;
}

int main(void)
{

	return 0;
}












