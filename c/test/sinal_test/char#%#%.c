#include <string.h>

int main()
{
	char *a[] = {"a","b","c","",""};
	char arr[] = "abc";

	printf("%d\n",sizeof(a)/sizeof(char *));
	return 0;
}
