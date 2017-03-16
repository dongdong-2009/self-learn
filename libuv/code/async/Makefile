LDFLAGS := -L../../libuv/
CFLAGS	:= -I../../libuv/include
LIB		:= -luv -lpthread
BIN		:= test

$(BIN) : main.o
	gcc $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIB)
	
.PHONY : clean

clean : 
	-rm -rf *.o $(BIN)
