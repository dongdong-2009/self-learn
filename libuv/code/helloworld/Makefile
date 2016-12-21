LDFLAGS := -L../../libuv/
CFLAGS	:= -I../../libuv/include
LIB		:= -llibuv

hello world : main.o
	gcc $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIB)
	
