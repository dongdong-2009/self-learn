all: httpd

httpd: httpd.c
	gcc -W -Wall -o httpd httpd.c -lpthread
#gcc -W -Wall -lsocket -lpthread -o httpd httpd.c

clean:
	rm httpd
