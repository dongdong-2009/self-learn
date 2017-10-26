#ifndef __IOTEKSTRING_H_
#define __IOTEKSTRING_H_

#include <stdio.h>

namespace iotek{
	class String{
		public:
			String(const char * = NULL);//带默认参数的构造函数
			~String();
			String(const String&);//拷贝构造函数,因有私有指针成员

			//String a;a=b;
			String& operator=(const String &);
			//String a;a="hello";
			String& operator=(const char *);

			//由于+=返回的是字符串本身，所以返回引用，不需要新创建对象
			String& operator+=(const String&);
			//+返回的是一个新的对象，所以返回时需要将匿名对象转正，即新创建了一个对象
			String operator+(const String&)const;

			String& operator+=(const char *);
			String operator+(const char*)const;

			inline const char* data()const
			{
				return m_data;
			}
		private:
			char *m_data;
	};
}

#endif
