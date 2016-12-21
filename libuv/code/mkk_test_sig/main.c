#include <stdlib.h>  
#include <stddef.h>  
#include <stdio.h>  
#include <string.h>  
#include <uv.h>  

int64_t counter = 0;    //空转计数器  
uv_idle_t idler;    //空转handle  

void signal_cb(uv_signal_t* handle, int signum);    //消息回调函数  
void wait_for_a_while(uv_idle_t* handle, int status);   //空转的回调函数  

/** 
 * 这儿我们关闭了系统所有的消息产生，程序就自动退出了 
 */  
void signal_cb(uv_signal_t* handle, int signum)  
{  
	printf("stop process!\n");  

	uv_idle_stop(&idler);   //停止空转  
	uv_close((uv_handle_t*)handle, NULL);   //停止消息监听  
}  

void wait_for_a_while(uv_idle_t* handle, int status)  
{  
	counter++;  
	printf("the counter is:%ld\n", counter);  
}  

int main()  
{  
	uv_signal_t sig;  
	uv_loop_t* loop;    //主消息循环  
	loop = uv_default_loop();  

	//监听消息  
	uv_signal_init(loop, &sig);  
	uv_signal_start(&sig, signal_cb, SIGINT);   //ctrl+c，对应SIGINT消息  

	//空转  
	uv_idle_init(loop, &idler);  
	uv_idle_start(&idler, wait_for_a_while);  

	uv_run(loop, UV_RUN_DEFAULT);  
	return 0;  
}
