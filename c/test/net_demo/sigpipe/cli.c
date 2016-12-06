#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	struct sockaddr_in serv_addr;
	int cli_sd;
	char buf[1024] = {'0'};

	cli_sd = socket(AF_INET,SOCK_STREAM,0);
	if(cli_sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	memset(&serv_addr,0x0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi("2000"));
	inet_pton(AF_INET,"192.168.100.124",&serv_addr.sin_addr);

	socklen_t len = sizeof(serv_addr);
	if(connect(cli_sd,(struct sockaddr *)&serv_addr,len) < 0)
	{
		perror("connect()");
		exit(1);
	}

	write(cli_sd,"abc",strlen("abc"));

	getchar();
	getchar();
	getchar();
	getchar();
	write(cli_sd,"abc",strlen("abc"));

	exit(0);
}
