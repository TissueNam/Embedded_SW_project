all: Hangman_client Hangman_server

Hangman_client: Hangman_client.c libhanglib.a hanglib.h
	gcc -o Hangman_client Hangman_client.c -lpthread -L../library -lhanglib -I../library

Hangman_server: Hangman_server.c libdefender.a defender.h libserverlib.a serverlib.h
	gcc -o Hangman_server Hangman_server.c -lpthread -L../labrary -ldefender -lserverlib
	
clean:
	rm -f -v Hangman_server
	rm -f -v Hangman_client
	rm -f -v Hangman_server.o
	rm -f -v Hangman_client.o	
