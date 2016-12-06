#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		printf("fork failed\n");
		return -1;
	}
	if(pid == 0)
	{
		sleep(1);
		printf("I am child process[child_id:%d][father_id:%d]\n",getpid(),getppid());
		return 0;
	}
	if(pid > 0)
	{
		printf("pid:%d\n",pid);
		printf("I am father,[child_id:%d][father_id:%d]\n",pid,getpid());
	}

	return 0;
}
