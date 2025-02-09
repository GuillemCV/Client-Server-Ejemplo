all: client server

client: codigo/client.c
	gcc -o ejecutables/client codigo/client.c

server: codigo/server.c
	gcc -o ejecutables/server codigo/server.c

clean:
	cd ejecutables && rm -f client server
