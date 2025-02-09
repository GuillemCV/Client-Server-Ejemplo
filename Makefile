all: client server socket_facade.o

socket_facade.o: socket_facade/socket_facade.c socket_facade/socket_facade.h
	gcc -c socket_facade/socket_facade.c -o socket_facade/socket_facade.o

client: codigo/client.c socket_facade/socket_facade.o
	gcc codigo/client.c socket_facade/socket_facade.o -o ejecutables/client

server: codigo/server.c socket_facade/socket_facade.o
	gcc codigo/server.c socket_facade/socket_facade.o -o ejecutables/server

clean:
	rm -f ejecutables/* socket_facade/socket_facade.o
