#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>				//fork
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define SERV_PORT		2048

//static sig_end_flag = 0;
static int listen_fd,conn_fd;

void sigchld(int s)
{
	printf("收尸\n");
	//wait会给当前进程的子进程收尸，没有处于僵尸状态的子进程，wait会阻塞等待收尸，没有子进程会返回-1；正常返回子进程的pid，参数为int指针可以获取子进程的状态
	wait(NULL);
}

void sig_handle(int sig)
{
#if 0
	sleep(2);
	printf("\n\nCTRL+C\n\n");
	close(listen_fd);
	close(conn_fd);
#endif
	//sig_end_flag = 1;
	printf("int.........\n");
	return;
}

int main(int argc,char **argv)
{
	pid_t child_pid;
	socklen_t chilen;
	struct sockaddr_in chiaddr,servaddr;
	char buf[1024] = {'0'};
	int set = 1;

	//signal(SIGINT,sig_handle);
	signal(SIGINT,SIG_DFL);
	//子进程结束时会给父进程发送SIGCHLD信号，但默认动作是忽略，那样子进程在调用exit之后会进入僵尸状态，在信号处理函数中，对该信号进行wait收尸可避免
	signal(SIGCHLD,sigchld);

	//1、创建socket
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd < 0)
	{
		perror("socket()");
		exit(-1);
	}


	//设置socket选项，一般来说一个端口号释放两分钟以后才能再被利用，SO_REUSEADDR是让端口号释放以后立即就能被再次利用
	if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&set,sizeof(set)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	//2、绑定socket
	memset(&chiaddr,0x0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr = htonl(atoi("192.168.100.121"));
	//servaddr.sin_port = htons(SERV_PORT);
	//servaddr.sin_port = htons(2048);
	inet_pton(AF_INET,"192.168.100.124",&servaddr.sin_addr);
	servaddr.sin_port = htons(atoi("2049"));
	if(bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
	{
		perror("bind()");
		exit(-1);
	}

	//3、开始监听socket
	if(listen(listen_fd,100) != 0)
	{
		perror("listen()");
		exit(-1);
	}

	//4、并发处理客户端的连接
	while(1)
	{
		chilen = sizeof(chiaddr);

		//accept会在客户端发送FIN报文时会阻塞，当server收到SIGINT信号时会打断该系统调用，由于没有设置SA_RESTART标志，所以无法重新调用阻塞的accpet系统调用（accept会返回EINTR错误）
		conn_fd = accept(listen_fd,(struct sockaddr*)&chiaddr,&chilen);
		if(conn_fd < 0)
		{
			if(conn_fd == EINTR)
			{
				printf("^-^我被SIGINT信号打断了\n");
			}
			perror("accept()");
			exit(-1);
		}
		printf("\n\nHave new client connect\n");

		//创建子进程处理连接
		child_pid = fork();
		if(child_pid < 0)
		{
			perror("fork()");
			exit(-1);
		}
		else if(child_pid == 0)
		{
			//进入子进程
			close(listen_fd);
			read(conn_fd,buf,1024);
			printf("[child_pid:%d]recv_data:%s\n",getpid(),buf);
			write(conn_fd,buf,strlen(buf));
			close(conn_fd);
			exit(0);
		}

		close(conn_fd);
	}
		
	
	close(listen_fd);
	printf("aaaaaaaaaaaaaaaaaa\n");

	exit(0);
}








