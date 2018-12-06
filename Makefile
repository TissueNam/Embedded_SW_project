all: Hangman_client Hangman_server

Hangman_client: Hangman_client.c
	gcc -o Hangman_client Hangman_client.c -lpthread

Hangman_server: Hangman_server.c
	gcc -o Hangman_server Hangman_server.c -lpthread
clean:
	rm -f -v Hangman_server
	rm -f -v Hangman_client
	rm -f -v Hangman_server.o
	rm -f -v Hangman_client.o
