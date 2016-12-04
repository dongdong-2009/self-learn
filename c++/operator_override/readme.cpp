c++语言规定：	
	1、创建的一个类会提供默认的拷贝构造函数和赋值运算符函数
	2、如果一个类提供了拷贝构造函数，那么也要提供一个重载的赋值运算函数
	3、重载运算符要保持原运算符的意义
	4、只能对已有的运算符重载，不能增加新的运算符
	5、重载的运算符不会改变原先的优先级和结合性
	6、使用成员函数比使用友元函数重载运算符，在定义上要少一个参数，因为有this指针
		如：
		RMB RMB::operator+(RMB& s)
		{
			uint int c = this->jf + s.jf;
			uint int d = this->yuan + s.yuan;
			return RMB(c,d);
		}
	7、c++规定：=、（）、[]、->这四个运算符必须作为成员函数重载

运算符重载的形式：
	1、成员函数
	2、友元函数

不能重载的运算符：
	::	.*	.	?:

c++规定参数说明都是内部类型时，不能重载，如：
	int* operator+(int,int*);//error


普通模型
	Person& Person::operate=(const Person &other)
	{
		if(this == &other)	//如果是自己则直接返回,对other的取地址，不是引用
			return this;
		//不是自己先释放原有的内存资源
		delete []m_data;

		//赋值运算的深拷贝
		int length = strlen(other.m_data);
		m_data = new char[length+1];
		strcpy(m_data,other.m_data);
		
		//赋值完毕，返回对象的引用
		return *this;			//返回this的实体，返回类型引用会直接接管该匿名（局部）实体
	}


使用友元重载一元运算符
class RMB{
public:
	RMB(unsigned int d,unsigned int c);
	friend RMB operator+(RMB&,RMB&);
	void display()
	{
		cout<<(yuan+jf/100.0)<<endl;
	}
private:
	unsigned int yuan;
	unsigned int jf;
};

代码实现
RMB operator+(RMB& s1,RMB& s2){
	//将元、角、分相加
	unsigned int jf = s1.jf + s2.jf;
	unsigned int yuan = s1.yuan + s2.yuan;
	//创建类使用带有两个参数的构造函数初始化类
	RMB result(yuan,jf);
	//返回初始化完的对象
	return result;
}

重载++运算符
	前++返回对象引用
	后++返回对象本身
	class Increase{
	public:
		Increase(int x)
			:value(x)
		{
		}
		Increase& operator++();//前++
		Increase operator++(int);//后++,int参数只是为了区别函数
	private:
		int value;
	}
	Increase& Increase::operator++(){
		value++;
		return *this;
	}
	Increase Increase::operator++(int)
	{
		Increase temp(*this);//调用默认的构造函数，将原对象保存
		value++;
		return temp;//返回原有对象值
	}
	注意：前++比后++效率高，少了一个对象的构建















