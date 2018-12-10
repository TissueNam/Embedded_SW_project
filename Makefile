all: Hangman_client Hangman_server

Hangman_client: Hangman_client.c libhanglib.a hanglib.h
	gcc -o Hangman_client Hangman_client.c -lpthread -L. -lhanglib -I.

libhanglib.a:hanglib.o
	ar rc libhanglib.a hanglib.o

hnaglib.o: hanglib.c
	gcc -c handlib.c

Hangman_server: Hangman_server.c libdefender.a defender.h libserverlib.a serverlib.h
	gcc -o Hangman_server Hangman_server.c -lpthread -L. -ldefender -lserverlib

libdefender.a:defender.o
	ar rc libdefender.a defender.o

defender.o: defender.c
	gcc -c defender.c

libserverlib.a:serverlib.o
	ar rc libserverlib.a serverlib.o

serverlib.o: serverlib.c
	gcc -c serverlib.c
	
clean:
	rm -f -v Hangman_server
	rm -f -v Hangman_client
	rm -f -v Hangman_server.o
	rm -f -v Hangman_client.o	
	rm -v hanglib.o
	rm -v defender.o
	rm -v serverlib.o
	rm *.a
