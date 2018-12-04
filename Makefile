all: client server

client: client.o
	gcc -o client client.o

server: server.o
	gcc -o server server.o

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c

clean:
	rm -f -v server
	rm -f -v client
	rm -f -v server.o
	rm -f -v client.o
	rm -f -v libfuncs.a
