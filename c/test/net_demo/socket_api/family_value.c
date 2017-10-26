#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
	printf("AF_INET:%d\nAF_INET6:%d\n",AF_INET,AF_INET6);
	return 0;
}
