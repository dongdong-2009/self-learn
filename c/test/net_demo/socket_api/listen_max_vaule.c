#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

//该函数执行失败，无法获取环境变量
//可以在/proc/sys/net/core/somaxconn查看当前系统设置的最大连接数
int main(void)
{
	char *ptr;
	int max_value;

	if(ptr = getenv("LISTENQ"))
	{
		max_value = atoi(ptr);
		printf("LISTENQ:%d\n",max_value);
	}
	else
	{
		printf("getenv error\n");
	}

	return 0;
}
