#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
	int sd = -1;
	int ret = -1;
	struct ifreq req;
	char mac[32] = {0};
	int i;  

	if(argc != 2){
		printf("Usage %s %s\n",argv[0],argv[1]);
		exit(1);
	}

	printf("interface:%s\n",argv[1]);

	sd = socket(PF_INET,SOCK_DGRAM,0);
	if(sd < 0){
		perror("socket error");
		exit(1);
	}

	strcpy(req.ifr_name, argv[1]);  

	ret = ioctl(sd, SIOCGIFADDR, &req);
	if(ret < 0){
		close(sd);
		perror("ioctl error");
		exit(1);
	}
	printf("IP: %s\n",inet_ntoa(((struct sockaddr_in*)&req.ifr_addr)->sin_addr)); 

	ret = ioctl(sd,SIOCGIFHWADDR,&req);
	if(ret < 0){
		close(sd);
		perror("ioctl error");
		exit(1);
	}

	for(i = 0; i < 6; ++i)  {  
		sprintf(mac + 3*i, "%02x:", (unsigned char)req.ifr_hwaddr.sa_data[i]);  
	}  
	printf("MAC: %s\n",mac);

	close(sd);
	exit(0);
}
