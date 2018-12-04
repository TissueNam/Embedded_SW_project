#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define PORT 7777   /* 포트 번호 */
#define MAXPENDING 5   /* 클라이언트 요구가 대기하는 큐의 크기 */
#define BUFSIZE 1024   /* 버퍼의 크기 */

main()
{
    int servSockfd;
    int clntSockfd;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    char recvBuffer[BUFSIZE];
    char sendBuffer[BUFSIZE];
    int clntLen;
    int recvLen;

    /* 소켓 생성 */
    if((servSockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("sock failed");
       exit(1);
    }

    /* servAddr를 0으로 초기화 */
    memset(&servAddr, 0, sizeof(servAddr));
    /* servAddr에 주소 정보 저장 */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    /* servSockfd 소켓에 주소 정보 연결 */
    if(bind(servSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
       perror("bind failed");
       exit(1);
    }

    /* servSockfd 소켓으로 들어오는 클라이언트 요청 기다림 */
    if(listen(servSockfd, MAXPENDING) == -1) {
       perror("listen failed");
       exit(1);
    }

    /* 무한 반복 */
    while(1) {
       clntLen = sizeof(clntAddr);
       /* servSockfd 소켓으로 들어오는 클라이언트 요청 받아들임 */
       if((clntSockfd=accept(servSockfd, (struct sockaddr*)&clntAddr, &clntLen)) == -1) {
          perror("accept failed");
          exit(1);
       }
       printf("client is in\n");
       /* 자식 프로세스 생성 */
       switch(fork()) {
          case -1:
             perror("fork failed");
             exit(1);

          /* 자식 프로세스는 */
          case 0:
             close(servSockfd);
             /* 임의의 클라이언트에 대해 반복 */
             while(1) {
                /* clntSockfd 소켓으로 들어오는 데이터를 받아 recvBuffer에 저장 */
                if((recvLen=recv(clntSockfd, recvBuffer, BUFSIZE, 0)) == -1) {
                   perror("recv failed");
                   exit(1);
                }
                /* 클라이언트가 연결을 끊으면 recv는 0을 반환 */
                if(recvLen == 0)
                   break;

                recvBuffer[recvLen] = '\0';
                /* 받은 데이터를 출력 */
                printf("%s\n", recvBuffer);
             }
             close(clntSockfd);
             exit(0);

          /* 부모 프로세스는 */
          default:
             while(1) {
                /* 터미널로부터 데이터 입력 */
                fgets(sendBuffer, BUFSIZE, stdin);

                /* 입력받은 데이터를 클라이언트에게 보냄 */
                if(send(clntSockfd, sendBuffer, strlen(sendBuffer), MSG_NOSIGNAL) != strlen(sendBuffer)) {
                   /* 상대방의 연결이 끊겨 호출에 실패하면 errno에는 EPIPE가 저장 */
                   if(errno == EPIPE) {
                      printf("client is out\n");
                      break;
                   }
                   perror("send failed");
                   exit(1);
                }
             }
       }
       close(clntSockfd);
    }
}

