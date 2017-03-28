main:main.o myutil.o
	gcc main.o myutil.o -o main

main.o:main.c myutil.h
	gcc -c main.c

myutil.o:myutil.c
	gcc -c myutil.c

clean:
	-rm -rf *.o main
