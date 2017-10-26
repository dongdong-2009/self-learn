#include "rmb.h"

RMB operator+(const RMB& val1,const RMB& val2)
{
	uint jf = val1.jf + val2.jf;
	uint yuan = val1.yuan + val2.yuan;
	if(jf < 10)
	{
		++yuan;
		jf -= 100;
	}

	//这里不能返回RMB的引用，返回引用会释放掉RMB创建的对象
	return RMB(yuan,jf);
}

bool	operator>(const RMB& val1,const RMB& val2)
{
	bool ret = false;
	if(val1.yuan > val2.yuan)
	{
		ret = true;
	}
	else if(val1.yuan == val2.yuan)
	{
		if(val1.jf > val2.jf)
		{
			ret = true;
		}
	}

	return ret;
}

RMB::RMB(uint y,uint val)
	:yuan(y),jf(val)
{

}

RMB::~RMB()
{

}
