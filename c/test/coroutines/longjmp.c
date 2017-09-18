#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf jumper;

static int _div(int a, int b) {
	if (b == 0) { // can't divide by 0
		longjmp(jumper, -1);//跳到以jumper 所在的jmp point，进行处理，-3 相当于具体的exception code.
	}
	return a / b;
}

int main(int argc, char *argv[]) {
	int jstatus = setjmp(jumper);//相当于java catch,如果发生 jumper 异常，那么会跳回到这个jmp point
	int a,b,res;

	while(1){
		if(jstatus == 0){
			system("clear");
			printf("please input a and b value\n");
			printf("a = ");
			scanf("%d",&a);
			printf("b = ");
			scanf("%d",&b);
			//printf("(your input:a = %d,b = %d)\n",a,b);
			res = _div(a,b);
			printf("a/b = %d\n",res);
		}
		else if(jstatus == -1){
			printf("[ERROR]Divide by zero\n");
		}else{
			printf("[OK]...\n");
		}
		jstatus = 0;
		getchar();
		getchar();
	}

#if 0
	if (jstatus == 0) {//第一次执行的时候是正确的setjmp return 0.
		int a = atoi(argv[1]);
		int b = atoi(argv[2]);
		printf("%d/%d", a, b);
		int result = _div(a, b);
		printf("=%d\n", result);
	} 
	else if (jstatus == -3)
		printf(" --> Error:divide by zero\n");
	else
		printf("Unhandled Error Case");
#endif

	return 0;
}
