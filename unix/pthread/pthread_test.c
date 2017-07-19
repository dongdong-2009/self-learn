#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>

static pthread_t pid[10];
static int pid_num[10] = {1,2,3,4,5,6,7,8,9,10};

void* thread(void *arg)
{
	prctl(PR_SET_NAME,"[akon------------]");

	int pid_node = *((int*)arg);
	printf("进入线程[%d]\n",pid_node);
	while(1){
		printf("Thread[%d] Running.\n",pid_node);
		sleep(1);
	}
	printf("离开线程[%d]\n",pid_node);
	pthread_exit(arg);
}

void* thread_4(void *arg)
{
	int pid_node = *((int*)arg);
	while(1){
		printf("Thread[%d] Running.\n",pid_node);
		break;
	}
	pthread_exit((void*)"hello");
}

static void test_pthread(int type)
{
	int ret = 0;
	int *join;
	void *cancel;

	switch(type){
		case 1:
			//线程结束返回的参数通过pthread_exit(void *arg)的参数传递
			pthread_create(&pid[0],NULL,thread,(void*)(pid_num+0));
			pthread_join(pid[0],(void**)(&join));
			printf("线程[%d]收尸\n",*join);
			break;
		case 2:
			//如果是cancel，则join参数的值为PTHREAD_CANCELED常数
			pthread_create(&pid[1],NULL,thread,(void*)(pid_num+1));
			sleep(3);
			pthread_cancel(pid[1]);
			pthread_join(pid[1],(void**)(&join));
			printf("线程收尸，join函数回填PTHREAD_CANCELED\n");
			break;
		case 3:
			pthread_create(&pid[2],NULL,thread,(void*)(pid_num+2));
			pthread_detach(pid[2]);
			//被分离的线程可以被cancel
			//pthread_cancel(pid[2]);
			//被分离的线程join没有意义,主线程不会挂起
			//pthread_join(pid[2],(void**)(&join));
			//被分离的线程主进程不能退出
#if 1
			while(1){
				printf("线程[3]结束\n");
				sleep(1);
			};
#endif
			break;
		case 4:
			pthread_create(&pid[3],NULL,thread_4,(void*)(pid_num+3));
			pthread_join(pid[3],&cancel);
			printf("线程[4]回收参数%s\n",(char*)cancel);
			//没有while会段错误
			while(1);
			break;
		default:
			printf("测试类型有问题\n");
			break;
	}
}

int main(int argc,char **argv)
{
	if(argc != 2){
		printf("Usage ./a.out 1|2|3....\n");
		exit(0);
	}
	test_pthread(atoi(argv[1]));

	return 0;
}

