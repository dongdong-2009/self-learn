#include "iotekstring.h"

#include <iostream>
using namespace std;
using namespace iotek;

//定义对象一定会调用构造函数（或拷贝构造函数）
int main(int argc,char **argv)
{
	String s1("hello");//调用构造函数

	String s2 = s1;//调用拷贝构造函数

	String s3 = "world";//调用构造函数，因为是定义对象

	s3 = s1;//调用运算符=重载函数
	s3 = "hello";//调用赋值运算符函数

	s1 += s3;
	cout << "s1 = " << s1.data() << endl;

	s3 += "!";

	String s4 = s1 + s2;

	s4 = s1 + "----";

	cout << "s3 = " << s3.data() << endl;
	return 0;
}
