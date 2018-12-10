#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
/*
void init_server_socket(int *serv_sock, struct sockaddr_in *serv_addr, char* port)
{
	//socket(int domain, int type, int protocol)
    *serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(*serv_sock==-1)
        error_handling("socket() error!");
	

    //bind()
    //struct Initialization (garbage value clear)
    memset(serv_addr, 0, sizeof(struct sockaddr_in));
    //struct serv_addr value
    serv_addr->sin_family=AF_INET;
    serv_addr->sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr->sin_port=htons(atoi(port));

    if(bind(*serv_sock, (struct sockaddr*)serv_addr, sizeof(*serv_addr))==-1)
        error_handling("bind() error!");

    if(listen(*serv_sock, 5)==-1)
        error_handling("listen() error!");

}
*/
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
void init_server_socket(int *serv_sock, struct sockaddr_in *serv_addr, char* port)
{
        //socket(int domain, int type, int protocol)
    *serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(*serv_sock==-1)
        error_handling("socket() error!");


    //bind()
    //struct Initialization (garbage value clear)
    memset(serv_addr, 0, sizeof(struct sockaddr_in));
    //struct serv_addr value
    serv_addr->sin_family=AF_INET;
    serv_addr->sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr->sin_port=htons(atoi(port));

    if(bind(*serv_sock, (struct sockaddr*)serv_addr, sizeof(*serv_addr))==-1)
        error_handling("bind() error!");

    if(listen(*serv_sock, 5)==-1)
        error_handling("listen() error!");

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
}
