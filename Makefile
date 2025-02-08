all:
	make client servidor

client:
	gcc -o client client.c

servidor:
	gcc -o servidor servidor.c

clean:
	rm -f client servidor
