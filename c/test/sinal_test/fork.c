#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int  hello(void)
{
	sleep(3);
	printf("================child pid is %d\n",getpid());
	return 0;
}

int main(void)
{
	pid_t pid,pid1;

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	else if(pid == 0)
	{
		hello();
		return 0;
	}

	pid1 = fork();
	if(pid1 < 0)
	{
		perror("fork()");
		exit(1);
	}
	else if (pid1 == 0)
	{
		while(1)
		{
			sleep(1);
			printf("=============>%d\n",pid);
		}
	}
	
	return 0;
}





