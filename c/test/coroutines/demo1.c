#include <stdio.h>

int func(void) 
{
	static int _cr_state = 0; 
	static int i = 0;

	switch(_cr_state) { 
		case 0:
			while(1){
				for(i = 1;i <= 10;i++) { 
						_cr_state = __LINE__ + 2;
						return i*i;
		case __LINE__:
					; 
			}
		}
	}
}

int main(void)
{
	int j = 0;

	for(j = 0; j < 10;j++){
		printf("get_value:%d\n",func());
	}

	return 0;
}
