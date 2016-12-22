#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <time.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  

#define SECONDS_1900_1970 0x83AA7E80 // 1900/1/1 至 1970/1/1 的秒数  

#define NTP_SRV_PORT 123  
#define NTP_PACKET_LEN 48  

#define TIMEOUT 1  

// 一些可用的NTP服务器  
//#define NTP_SERVER_0 "218.75.4.130"  
#define NTP_SERVER_0 "202.120.2.101"  
#define NTP_SERVER_1 "114.80.81.1"  
#define NTP_SERVER_2 "202.120.2.101"  
#define NTP_SERVER_3 "210.72.145.44"  

static void construct_ntp_packet(char content[])  
{  
	memset(content, 0, NTP_PACKET_LEN);  
	content[0] = 0x1b; // LI(2bit) = 0; VN(3bit) = 3; Mode(3bit) = 3 (client); 00 011 011 -> 00011011 -> 0x1b  
}  

int get_ntp_time(int sockfd, struct sockaddr_in *server_addr, struct tm *net_tm)  
{  
	char content[256];  
	time_t timet;  
	long temp;  
	int addr_len = 16;  
	struct timeval block_time;  
	fd_set sockfd_set;  

	FD_ZERO(&sockfd_set);  
	FD_SET(sockfd, &sockfd_set);  
	block_time.tv_sec = TIMEOUT; //recv timeout  
	block_time.tv_usec = 0;  

	construct_ntp_packet(content);  
	if (sendto(sockfd, content, NTP_PACKET_LEN, 0, (struct sockaddr *)server_addr, addr_len) < 0)  
	{  
		perror("sendto error");  
		return (-1);  
	}  

	if (select(sockfd + 1, &sockfd_set, NULL, NULL, &block_time ) > 0)  
	{  
		memset(content, 0, sizeof(content));  
		if (recvfrom(sockfd, content, sizeof(content), 0, (struct sockaddr *)server_addr, (socklen_t *)&addr_len) < 0)  
		{  
			perror("recvfrom error");  
			return (-1);  
		}  
		else  
		{  
			memcpy(&temp, content + 40, 4); //提取服务器端设置的transmit_timastamp  

			temp = (time_t)(ntohl(temp) - SECONDS_1900_1970);  
			timet = (time_t)temp;  
			memcpy(net_tm, gmtime(&timet), sizeof(struct tm));  
			net_tm->tm_hour = net_tm->tm_hour + 8; //北京时间，东八区  
		}  
	}  
	else  
	{  
		return (-1);  
	}  

	return (0);  
}  

int main(void)  
{  
	int sockfd, i;  
	struct tm *net_tm;  
	struct sockaddr_in addr;  
	char ip[4][16]= {{NTP_SERVER_0}, {NTP_SERVER_1}, {NTP_SERVER_2}, {NTP_SERVER_3}};  

	net_tm = (struct tm *)malloc(sizeof(struct tm));  

	for (i = 0; i < 4; i++)  
	{  
		memset(&addr, 0, sizeof(addr));  
		addr.sin_addr.s_addr = inet_addr(ip[i]);  
		addr.sin_port = htons(NTP_SRV_PORT);  

		if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)  
		{  
			perror("socket error");  
			return (-1);  
		}  

		printf("IP:[%s]\n", ip[i]);  
		memset(net_tm, 0, sizeof(struct tm));  
		if (get_ntp_time(sockfd, &addr, net_tm) == 0)  
		{  
			printf("NTP:[%d/%d/%d %d:%d:%d]\n\n",  
					net_tm->tm_year+1900,  
					net_tm->tm_mon+1,  
					net_tm->tm_mday,  
					net_tm->tm_hour,  
					net_tm->tm_min,  
					net_tm->tm_sec);  
		}  
		else  
		{  
			printf("failed!!!\n\n");  
		}  

		close(sockfd);  
	}  

	return (0);  
}  
