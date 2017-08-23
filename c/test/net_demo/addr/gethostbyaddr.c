#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
	struct hostent *host;
	int addr;

	host = gethostbyaddr(inet_pton(AF_INET,argv[1],&addr),4,AF_INET);
	if(host == NULL)
	{
		printf("gethostbyaddr error\n");
		exit(1);
	}
	printf("%s-------->%s\n",argv[1],host->h_name);

	exit(0);
}
