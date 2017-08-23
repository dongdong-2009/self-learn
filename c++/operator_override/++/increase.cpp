#include "increase.h"
#include <iostream>
using namespace std;

Increase::Increase(int val)
	:m_val(val)
{

}

Increase::~Increase()
{
}

Increase& Increase::operator++()
{
	++m_val;
	return *this;
}

Increase Increase::operator++(int)
{
	Increase temp(m_val);
	++m_val;
	return temp;
}
