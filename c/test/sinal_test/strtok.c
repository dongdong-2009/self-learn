#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
	char str[30] = "hello_world_mkk";
	char *p = NULL;

	p = strtok(str,"_");
	if(p)
		printf("[1]----->%s\n",p);
	else
		printf("error");
	printf("[2]----->%s\n",strtok(NULL,"_"));
	printf("[3]----->%s\n",strtok(NULL,"_"));
	printf("[4]----->%s\n",strtok(NULL,"_"));
	return 0;
}
