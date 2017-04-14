#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//宏替换中的#是将#后面的待替换标识子符合窜化，但是需要用双引号包含在两侧
#define WARNING(EXP)	while(1){\
							printf("Warning:"#EXP"\n");\
							sleep(1);\
						}


int main(void){
	WARNING(This is a warning);
		
	return 0;
}
