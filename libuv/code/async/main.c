#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <uv.h>

uv_async_t async;
uv_loop_t *loop;
uv_thread_t	new;

void close_cb(uv_handle_t *handle){
	printf("close the async handle.\n");
}

void async_cb(uv_async_t *handle,int status){
	printf("async_cb called\n");
	uv_thread_t id = uv_thread_self();
	printf("thread id:%lu.\n", id);  
	//打开下面将会导致uv_run结束，整个时间循环结束,只能send一次，否则可以send多次
	//uv_close((uv_handle_t*)&async, close_cb);   //如果async没有关闭，消息队列是会阻塞的 
}

void* new_thread(void *args){
	int i = 0;
	while(1){
		i++;
		//printf("=============>%d\n",i);
		if(i % 3 == 0){
			//每隔3妙send一次，每次send只要不uv_close，都会执行async绑定的回调函数
			uv_async_send(&async);  
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc,char **argv)
{
	loop = uv_default_loop();  

	uv_thread_t id = uv_thread_self();  
	printf("thread id:%lu.\n", id);  

	uv_async_init(loop, &async, async_cb);
	
	//创建线程去uv_async_send
	if(pthread_create(&new,NULL,new_thread,NULL) < 0){
		printf("xxxxxxxxxxxxxxxxxxxxxxxx\n");
		exit(1);
	}

	//uv_async_send(&async);  
	uv_run(loop, UV_RUN_DEFAULT);  
	
	//执行不到
	printf("-------------------------\n");
	
	pthread_join(new,NULL);

	return 0;
}





