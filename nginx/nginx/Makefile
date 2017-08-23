
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/home/fattiger/nginx-1.6.2/nginx -t

	kill -USR2 `cat /home/fattiger/nginx-1.6.2/nginx.pid`
	sleep 1
	test -f /home/fattiger/nginx-1.6.2/nginx.pid.oldbin

	kill -QUIT `cat /home/fattiger/nginx-1.6.2/nginx.pid.oldbin`
