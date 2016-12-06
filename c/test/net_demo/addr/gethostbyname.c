#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
	struct hostent  *my_host;
	char addr[64];
	char **ptr;

	my_host = gethostbyname(argv[1]);
	if(!my_host)
	{
		perror("gethostbyname()");
		exit(1);
	}

	//printf("offical_name:%s\n",inet_ntop(AF_INET,my_host->h_name,addr,64));
	//printf("h_name:[%s]\th_aliases[1]:[%s]\n",my_host->h_name,my_host->h_aliases[1]);
	//printf("-------------->%ld\n",sizeof(my_host->h_aliases)/sizeof(char*));
	//inet_ntop(AF_INET,my_host->h_addr);
	if(my_host->h_addrtype == AF_INET)
	{
		for(ptr = my_host->h_addr_list;*ptr != NULL;ptr++)
		{
			printf("%s:%s\n",argv[1],inet_ntop(AF_INET,ptr,addr,64));
		}
	}

	exit(0);
}
