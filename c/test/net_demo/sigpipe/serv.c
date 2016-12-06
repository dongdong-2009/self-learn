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


int main(int argc,char **argv)
{
	int serv_sd,new_sd;
	struct sockaddr_in serv_addr,cli_addr;
	int val = 1;
	char buf[1024] = {'0'};

	serv_sd = socket(AF_INET,SOCK_STREAM,0);
	if(serv_sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	if(setsockopt(serv_sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	memset(&serv_addr,0x0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi("2000"));
	inet_pton(AF_INET,"192.168.100.124",&serv_addr.sin_addr);
	if(bind(serv_sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	if(listen(serv_sd,100) < 0)
	{
		perror("listen()");
		exit(1);
	}
	//printf("listen endl\n");


	int len = sizeof(cli_addr);
	new_sd = accept(serv_sd,(struct sockaddr *)&cli_addr,&len);
	if(new_sd < 0)
	{
		perror("new_sd()");
		//close(serv_sd);
		exit(1);
	}

	read(new_sd,buf,1024);
	printf("recvdata:%s\n",buf);

	close(new_sd);
	close(serv_sd);

	exit(0);
}





