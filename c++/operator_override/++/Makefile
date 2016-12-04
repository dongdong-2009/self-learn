exec=increase
cc=g++


.PHONY:clean

$(exec) : main.o increase.o
	$(cc) $^ -o $@

clean:
	rm -rf *.o ${exec}
