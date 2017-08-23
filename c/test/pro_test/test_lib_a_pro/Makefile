EXEC = aaaa

CFLAGS := -I./include
LDFLAGS := -L./lib
LDLIBS := ./lib/libdemo.a
CC = gcc


$(EXEC) : ./src/main.o
	$(CC) $(LDLIBS) $(CFLAGS) -o $@ $^ $(LDLIBS)

./src/main.o : ./src/main.c

clean :
	rm -rf $(EXEC)
