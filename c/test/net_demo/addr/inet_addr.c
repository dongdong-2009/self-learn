#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	printf("主机字节序:%s------->网络字节序:0x%x\n","255.255.255.255",inet_addr("255.255.255.255"));
	return 0;
}
