#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <uv.h>

//#define	hello
//#define	idle

#ifdef hello
int main() 
{
	//uv_loop_t *loop = uv_loop_new();		//创建一个新的事件环
	uv_loop_t *loop = uv_default_loop();	//libuv默认的事件环，nodejs主线程就是用该事件环
	printf("Now quitting.\n");

	//以默认模式启动事件环，因为没有要监听的事件所以程序会直接退出
	uv_run(loop,UV_RUN_DEFAULT);
	return 0;
}
#endif

#ifdef idle
int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle, int status) {
    counter++;

	printf("=======>%ld[%d]\n",counter,status);
    if (counter >= 10)
        uv_idle_stop(handle);
}

int main() {
    uv_idle_t idler;

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling...\n");
	//当事件环中没有事件要监听时循环停止
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	printf("================================\n");
    return 0;
}
#endif


#define async
#ifdef async
uv_async_t async;
uv_loop_t *loop;

void close_cb(uv_handle_t *handle){
	printf("close the async handle.\n");
}

void async_cb(uv_async_t *handle,int status){
	printf("async_cb called\n");
	uv_thread_t id = uv_thread_self();
	printf("thread id:%lu.\n",id);
}

int main(int argc,char **argv)
{
			
	return 0;
}
#endif



















