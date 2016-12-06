CC = gcc
OBJ = ../build/obj/demo.o
LIB = ../lib/libdemo.a

.PHONY : clean all

all : libdemo.a

libdemo.a : demo.o
	ar rcs -o $(LIB) $(OBJ)

demo.o : demo.c
	$(CC) -c demo.c -o $(OBJ)

clean :
	rm -rf $(OBJ) $(LIB)
