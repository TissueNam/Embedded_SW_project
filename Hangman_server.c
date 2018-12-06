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

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
 
    struct epoll_event event;
    struct epoll_event *events;
    int epfd;
    int event_cnt, i;
 
    int user_cnt=0;        //user count
    int user[2]={0};    //user information = [ socket ]
    int game_flag=0;    // Play game or not
    int challenger[2]={0};    //challenger [0]=socket [1]=flag
    int examiner[2]={0};    //examiner [0]=socket [1]=flag
    char word[BUF_SIZE];    //answer
    char question[BUF_SIZE];    //challenger's word
    int hint_cnt, hang_cnt, j, iscorrect, iscontinue;
    
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    // Memory 
    events = malloc(sizeof(struct epoll_event)*50);

    //socket(int domain, int type, int protocol)
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock==-1)
        error_handling("socket() error!");

    //bind()
    //struct Initialization (garbage value clear)
    memset(&serv_addr, 0, sizeof(serv_addr));
    //struct serv_addr value
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error!");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error!");

    //accept()
    clnt_addr_size=sizeof(clnt_addr);

    epfd=epoll_create(50);    // epoll (event registration / Max : 50)
    event.events=EPOLLIN;    // event type
    event.data.fd=serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);    //event registration

}

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
}
