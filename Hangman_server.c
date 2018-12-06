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

      while(1)
    {
        printf("%d\n", serv_sock);
        event_cnt=epoll_wait(epfd, events, 50, -1); // return lists

        if(event_cnt==-1)    // Terminate if error
            error_handling("epoll_wait() error!\n");
            //break;

        printf("%d\n", event_cnt);
        for(i=0; i<event_cnt; i++)
        {
            if(events[i].data.fd==serv_sock) // check socket is listening
            {    //accept
                clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                if(clnt_sock==-1)
                    error_handling("accept() error!");
                else
                    printf("connected client: %d\n", clnt_sock);
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);

                user_cnt++;
                if(user_cnt<=2){
                    user[user_cnt-1]=clnt_sock;
                    strcpy(message, "Welcome to the Hangman game.");
                    if(user_cnt==1){
                        strcat(message, "1\n Game will be started when player comes\n");
                    }
                    else if(user_cnt==2){
                        strcat(message, "2\nGame will be started soon..\n");
                        send(user[0], "Game will be started..\n", strlen("Game will be started..\n"), 0);
                        game_flag=1;
                    }
                    send(clnt_sock, message, strlen(message), 0);
                }
		 else{    // if 2 players come
                    send(clnt_sock, "fail", strlen("fail"), 0);
                }
                    if(game_flag==1&&user_cnt==2){    // Game start
                    hint_cnt=HINT; hang_cnt=0;
                    strcpy(message, "Who is the examiner..?\n");
                    send(user[0], message, strlen(message), 0);
                    // create random number
                    srand(time(NULL));
                    examiner[0]=user[rand()%2];
                    if(examiner[0]==user[0])
                        challenger[0]=user[1];
                    else
                        challenger[0]=user[0];
                    strcpy(message, "Player's turn!\n");
                    send(challenger[0], message, strlen(message), 0);
                    strcpy(message, "Please wait until examiner makes quiz\n");
                    send(challenger[0], message, strlen(message), 0);
                    strcpy(message, "Your turn!\n");
                    send(examiner[0], message, strlen(message), 0);
                    strcpy(message, "Enter the your word in lower-case alpahbetic characters : ");
                    send(examiner[0], message, strlen(message), 0);
                    examiner[1]=1;
                }
            }
	}

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
