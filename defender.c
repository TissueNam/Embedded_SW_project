#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/times.h>
#include <sys/epoll.h>

#include "defender.h"

#define BUF_SIZE 1024
#define PLAYER 2
#define HINT 3



char* event_1_make_quiz(char* msg)
{
	char word[BUF_SIZE];
	strcpy(word, msg);
	printf("%s\n",word);
	strcpy(msg, "**Quiz : ");
	strcat(msg, word);
	strcat(msg, "\nIntical Hint plz. (Category or hint, 'spyderman' is 'hero') : ");
	return msg;	
}
