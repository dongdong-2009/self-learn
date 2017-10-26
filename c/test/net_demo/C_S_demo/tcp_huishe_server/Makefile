all:client mkk_server

client:client.c
	gcc $^ -o $@
mkk_server:server.c
	gcc $^ -o $@

clean:
	rm -rf client mkk_server
