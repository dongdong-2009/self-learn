#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
 
#define BUF_SIZE 30 
 
/*
   创建一对匿名socket，只能使用AF_UNIX域
   读s[0]或s[1]会阻塞，一对socket都可以读写，全双工
   配对方式：写1读0,写0读1
 */
int main(){ 
	int s[2]; 
	int w,r; 
	char * string = "This is a test string"; 
	char * buf = (char*)calloc(1 , BUF_SIZE); 
									 
	if( socketpair(AF_UNIX,SOCK_STREAM,0,s) == -1 ){ 
	    printf("create unnamed socket pair failed:%s\n",strerror(errno) ); 
	    exit(-1); 
	} 
											 
	/*******test in a single process ********/ 
	if( ( w = write(s[0] , string , strlen(string) ) ) == -1 ){ 
	    printf("Write socket error:%s\n",strerror(errno)); 
	    exit(-1); 
	} 
	/*****read*******/ 
	if( (r = read(s[1], buf , BUF_SIZE )) == -1){ 
	    printf("Read from socket error:%s\n",strerror(errno) ); 
	    exit(-1); 
	} 
	printf("Read string in same process : %s \n",buf); 

#if 1
	//如果此处不写1,下面的读0会失败
	if( ( w = write(s[1] , string , strlen(string) ) ) == -1 ){ 
	    printf("Write socket error:%s\n",strerror(errno)); 
	    exit(-1); 
	} 
#endif

	if( (r = read(s[0], buf , BUF_SIZE )) == -1){ 
	    printf("Read from socket s0 error:%s\n",strerror(errno) ); 
	    exit(-1); 
	} 
	printf("Read from s0 :%s\n",buf); 

	printf("Test successed\n"); 
	exit(0); 
} 
