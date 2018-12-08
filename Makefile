all: Hangman_client Hangman_server

Hangman_client: Hangman_client.c libhanglib.a hanglib.h
	gcc -o Hangman_client Hangman_client.c -lpthread -L. -lhanglib -I.

libhanglib.a:hanglib.o
	ar rc libhanglib.a hanglib.o

hnaglib.o: hanglib.c
	gcc -c handlib.c
Hangman_server: Hangman_server.c
	gcc -o Hangman_server Hangman_server.c -lpthread

clean:
	rm -f -v Hangman_server
	rm -f -v Hangman_client
	rm -f -v Hangman_server.o
	rm -f -v Hangman_client.o	
	rm -v hanglib.o
	rm *.a
