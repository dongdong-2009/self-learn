#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char **argv)
{
	int client_sd;
	struct sockaddr_in serv_addr;
	socklen_t len;
	char buf[1024] = {'0'};

	client_sd = socket(AF_INET,SOCK_STREAM,0);
	if(client_sd < 0)
	{
		perror("socket()");
		exit(-1);
	}

	memset(&serv_addr,0x0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl(atoi("192.168.100.121"));
	inet_pton(AF_INET,"192.168.100.124",&serv_addr.sin_addr);
	serv_addr.sin_port = htons(atoi("2049"));
	len = sizeof(serv_addr);

	if(connect(client_sd,(struct sockaddr *)&serv_addr,len) < 0)
	{
		perror("connect()");
		exit(-1);
	}

	fgets(buf,1024,stdin);
	write(client_sd,buf,strlen(buf));
	read(client_sd,buf,1024);
	printf("recv_data:%s\n",buf);

	close(client_sd);

	return 0;
}








