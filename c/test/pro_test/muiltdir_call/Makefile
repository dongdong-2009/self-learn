CC = gcc

CFLAGS	:= -I./include


.PHONY:clean


hello:main.o hello.o world.o
	${CC} $^ -o $@

main.o:main.c
	${CC} ${CFLAGS} -c $< -o $@

hello.o:./src/hello/hello.c
	${CC} ${CFLAGS} -c $< -o $@

world.o:./src/world/world.c
	${CC} -c $< -o $@

clean:
	rm -rf *.o  hello
