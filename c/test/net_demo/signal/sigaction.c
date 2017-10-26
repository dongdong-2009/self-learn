#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void handle(int s)
{
	printf("----------------\n");
}

int main(void)
{
	struct sigaction act,oact;

	act.sa_handler = handle;
	sigemptyset(&act.sa_mask);
	//信号执行一次之后按照默认的行为执行
	act.sa_flags = SA_RESETHAND;

	if(sigaction(SIGINT,&act,&oact) < 0)
	{
		perror("sigaction()");
		exit(1);
	}

	while(1)
	{
	}

	return 0;
}
