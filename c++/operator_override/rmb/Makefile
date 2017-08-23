exec = rmb
cc = g++

.PHONY:clean

exec:main.o rmb.o
	$(cc) -o ${exec} $^

clean:
	rm -rf *.o $(exec)
