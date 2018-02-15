.PHONY : all clean

CFLAGS := -I./			#定义头文件包含路径
LDFLAGS :=				#定义库文件包含路径
BIN := exe
CC := gcc

all : ${BIN}

${BIN} : *.o
	${CC} ${CFLAGS} $^ -o $@
 

%.o : %.c
	${CC} ${CFLAGS} -c $^


clean :
	echo "\33[0m"
	-rm -rf *.o ${BIN}
