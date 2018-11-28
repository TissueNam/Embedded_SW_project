#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define PORT 5000
#define MAXPENDING 5
#define BUFSIZE 1024

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

	/* make socket */
	if((servSockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("sock failed");
		exit(1);
	}

	/* make servAddr to zero */
       memset(&servAddr, 0, sizeof(servAddr));
	/* save address in servAddr */
	servAddr.sin_family = AF_INET;
 	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	servAddr.sin_port = htons(PORT);

	/* connect servSockfd socket with address infomation */
	if(bind(servSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1 ) {
		perror("bind failed");
		exit(1);
	}

	/* wait demand of client comging to servSockfd */
	if(listen(servSockfd, MAXPENDING) == -1) {
			perror("listen failed");
			exit(1);
			}

	/* infinite */
	while(1) {
		clntLen = sizeof(clntAddr);
		/* accept demand of client coming to servSockfd */
		if((clntSockfd=accept(servSockfd, (struct sockaddr*)&clntAddr, &clntLen)) == -1) {
			perror("accept failed");
			exit(1);
		}
		printf("client is in\n");
		/* make child process */
		switch(fork()) {
			case -1:
				perror("fork failed");
				exit(1);

			/* child process is.. */
			case 0:
				close(servSockfd);
				/* recursive (in any clients) */
				while(1) {
					/* Save data of coming to clntSockfd and Save it recvBuffer */
					if((recvLen=recv(clntSockfd, recvBuffer, BUFSIZE, 0)) == -1) {
						perror("recv failed");
						exit(1);
					}
					/* recv returns zero when client disconnect */
					if(recvLen == 0)
						break;

					recvBuffer[recvLen] = '\0';
					/* print received data */
					printf("%s\n", recvBuffer);
				}
				close(clntSockfd);
				exit(0);

			/* parent process is.. */
			default:
				while(1) {
					/* enter data from Terminal */
					fgets(sendBuffer, BUFSIZE, stdin);

					/* send entered data to client */
					if(send(clntSockfd, sendBuffer, strlen(sendBuffer), MSG_NOSIGNAL) != strlen(sendBuffer)) {
						/* when it's disconected with the other person, save EPIPE in errno */

						if(errno = EPIPE) {
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


				










