#include "animal.h"
using namespace iotek;

int main(void)
{
	Cat cat(1,1,"beijing");
	cat.setAge(2);
	cat.setColor(2);

	Dog dog(2,2,"shanghai");
	dog.setAge(3);
	dog.setWeight(3);

	return 0;
}
