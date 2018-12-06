#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#include <sys/time.h>
#include <sys/epoll.h>

#define BUF_SIZE 1024

#define HINT 3

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

char* drawHangman(int num){
    char* hangman={ 0 };
    switch (num)
    {
        case 0:
        hangman = "┌───┐\n│\n│\n│\n│\n└──────\n";
        break;
        case 1:
        hangman = "┌───┐\n│　○\n│\n│\n│\n└──────\n";
        break;
        case 2:
        hangman = "┌───┐\n│　○\n│　 |\n│\n│\n└──────\n";
        break;
        case 3:
        hangman = "┌───┐\n│　○\n│　/|\n│\n│\n└──────\n";
        break;
        case 4:
        hangman = "┌───┐\n│　○\n│　/|＼\n│　\n│\n└──────\n";
        break;
        case 5:
        hangman = "┌───┐\n│　○\n│　/|＼\n│　/\n│\n└──────\n";
        break;
        case 6:
        hangman = "┌───┐\n│　○\n│　/|＼\n│　/＼\n│\n└──────\n";
        break;
        case 7:
        hangman = "┌───┐\n│　○\n│　 X\n│　/|＼\n│　/＼\n└──────\n";
        break;
        default:
        hangman = "drawing error\n";
        break;
    }
