#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(void)
{
	struct hostent *host_info;
	char str[16];

	host_info = gethostbyname("127.0.0.1");
	if(!host_info)
	{
		printf("error\n");
	}

	
	printf("h_name:%s,h_addr_list:%s\n",host_info->h_name,inet_ntop(AF_INET,host_info->h_addr_list[0],str,16));
	return 0;
}
