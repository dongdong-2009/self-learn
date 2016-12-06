#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc,char **argv)
{
	struct servent *service;

	service = getservbyname(argv[1],"tcp");
	printf("%s port:%d\n",argv[1],ntohs(service->s_port));

	exit(0);
}
