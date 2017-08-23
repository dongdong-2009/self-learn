#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
pid_t gettid()
{
    return syscall(SYS_gettid);
}
int main(int argc,char *argv)
{
    printf("gettid = %d\n",gettid());
    printf("pthread_self=%ld\n",pthread_self());
    return 0;
}
