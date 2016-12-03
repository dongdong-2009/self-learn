#ifndef _SIGNLETON_H_
#define _SIGNLETON_H_

//例如framebuffer，服务管理，只需要创建一个对象，然后去做一些事情
class Singleton{
	public:
		static Singleton* getInstance();
		void doSomething();
		void destory();
	private:
		Singleton();
		~Singleton();

		Singleton(const Singleton&);				//拷贝构造函数
		Singleton& operator=(const Singleton &);	//运算符重载
		static Singleton* instance;
};

#endif
