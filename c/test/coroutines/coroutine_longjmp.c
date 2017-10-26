#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <math.h>

jmp_buf jumper;
int mmax;

int next(void)
{
	static int num;

	num++;
	if(num >= mmax){
		longjmp(jumper,-1);
	}else{
		longjmp(jumper,num*num);
	}
}

void _next(int max)
{
	static int once;

	if(!once){
		once = 1;
		mmax = max;
	}
	
	next();
}

int main(int argc,char **argv)
{
	int jstatus;

	printf("start\n");
	jstatus = setjmp(jumper);
#if 0
	if(jstatus == 0){
		_next();
	}else if(jstatus == -1){
		printf("!!!!!end!!!!!!!\n");
	}else{
		printf("[ok][long_ret:%d]end\n",jstatus);
	}
	return 0;
#endif

	while(1){
		if(jstatus == 0){
			_next(atoi(argv[1]));
		}else if(jstatus == -1){
			printf("end\n");
			break;
		}else{
			printf("--->%d\n",jstatus);	
			jstatus = 0;
		}
	}

	return 0;
}



