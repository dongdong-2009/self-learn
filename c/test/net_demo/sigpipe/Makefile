.PONHY = clean

all : ser cli

ser : serv.c
	gcc $^ -o $@
cli : cli.c
	gcc $^ -o $@

clean:
	-rm -rf ser cli
