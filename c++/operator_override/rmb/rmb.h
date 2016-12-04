#ifndef _RMB_H
#define _RMB_H

typedef unsigned int uint;

#include <iostream>
using namespace std;

class RMB{
	friend RMB operator+(const RMB&,const RMB&);
	friend bool operator>(const RMB&,const RMB&);
public:
	RMB(uint y,uint jf);
	~RMB();

	void display()const{	//在成员函数后面加const，实际上是给this指针加上了this修饰
		cout << "RMB:" << yuan << ".";
		if(jf < 10){
			cout << "0";
		}
		cout << jf << endl;
	}
private:
	uint yuan;
	uint jf;
};

#endif
