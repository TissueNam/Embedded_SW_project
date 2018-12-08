#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 7777   /* 서버의 포트 번호 */
#define BUFSIZE 1024   /* 버퍼의 크기 */

void signalHandler(int signo);
int sockfd;

/* argv[1]은 수와 점 표기의 IP 주소 */
int main(int argc, char *argv[])
{
    struct sockaddr_in servAddr;
    char sendBuffer[BUFSIZE], recvBuffer[BUFSIZE];
    int recvLen;
    pid_t pid;

    if(argc != 3) {
       fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
       exit(1);
    }

    /* 소켓 생성 */
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("sock failed");
       exit(1);
    }
    /* servAddr을 0으로 초기화 */
    memset(&servAddr, 0, sizeof(servAddr));
    /* servAddr에 주소 정보 저장 */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    /* servAddr 서버에 통신을 요청 */
    if(connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
       perror("connect failed");
       exit(1);
    }

    /* 자식 프로세스 생성 */
    switch(pid=fork()) {
       case -1:
          perror("fork failed");
          exit(1);

       /* 자식 프로세스는 */
       case 0:
          /* SIGINT 시그널 받으면 signalHandler 실행하도록 설정 */
          signal(SIGINT, signalHandler);
          /* SIGINT 시그널 받을 때까지 반복 */
          while(1) {
             /* sockfd 소켓으로 들어오는 데이터를 받아 recvBuffer에 저장 */
             if((recvLen=recv(sockfd, recvBuffer, BUFSIZE, 0)) == -1) {
                perror("recv failed");
                exit(1);
             }

             recvBuffer[recvLen] = '\0';
             /* 받은 데이터를 출력 */
             printf("%s\n", recvBuffer);
          }
          break;

       /* 부모 프로세스는 */
       default:
          /* quit를 입력 받을 때까지 반복 */
          while(1) {
             /* 터미널로부터 데이터 입력 */
             fgets(sendBuffer, BUFSIZE, stdin);

             if(!strncmp(sendBuffer, "quit", 4)) {
                /* 자식 프로세스에게 SIGINT 시그널 보냄 */
                kill(pid, SIGINT);
                break;
             }

             /* 입력받은 데이터를 서버에게 보냄 */
             if(send(sockfd, sendBuffer, strlen(sendBuffer), 0) != strlen(sendBuffer)) {
                perror("send failed");
                exit(1);
             }
          }
    }
    close(sockfd);
    exit(0);
}

/* SIGINT 시그널 받으면 실행될 핸들러 */
void signalHandler(int signo)
{
    close(sockfd);
    exit(0);
}

