#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	char *tmp;
	int len = strlen(argv[1]);

	if(strstr(argv[1],"/"))
		printf("%s\n",strstr(argv[1],"/"));
	else
		printf("--->%s\n",argv[1]);
#if 0
	//if(strstr(argv[1],"/"))
	{
		while(argv[1][len - 1] != '/')
		{
			if(len == 0)
				break;
			len--;
		}
	}
	printf("%s\n",argv[1]+len);
#endif
#if 0
	tmp = argv[1];
	while(1)
	{
		if(!(strstr(tmp,"/")))
		{
			printf("%s\n",tmp);
			break;
		}
		tmp = strstr(tmp,"/");
	}
#endif
	return 0;
}
