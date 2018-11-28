#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 5000
#define BUFSIZE 1024

void signalHandler(int signo);
int sockfd;

/* argv[1] is ip address of the number and point */
main(int argc, char *argv[])
{
	struct sockaddr_in servAddr;
	char sendBuffer[BUFSIZE], recvBuffer[BUFSIZE];
	int recvLen;
	pid_t pid;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
		exit(1);
	}

	/* make socket */
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("sock failed");
		exit(1);
	}

	/* make servAddr to zero */
	memset(&servAddr, 0, sizeof(servAddr));
	/* save address in servAddr */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(PORT);

	/* ask to communicate on servAddr server */
	if(connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
		perror("connect failed");
		exit(1);
	}

	/* make child process */
	switch(pid=fork()) {
		case -1:
			perror("fork failed");
			exit(1);

		/* child process is.. */
		case 0:
			/* set to execute signalHandler when it takes SIGINT signal */
			signal(SIGINT, signalHandler);
			/* recursive until it takes SIGINT */
			while(1) {
				/* take data coming sockfd socket and save recvBuffer */
				if((recvLen=recv(sockfd, recvBuffer, BUFSIZE, 0)) == -1 ) {
					perror("recv failed");
					exit(1);
				}

				recvBuffer[recvLen] = '\0';
				/* print recieved data */
				printf("%s\n", recvBuffer);
			}
			break;

		/* parent process is.. */
		dafault:
			/* recursive until it takes 'quit' */
			while(1) {
				/* insert data from Terminal */
				fgets(sendBuffer, BUFSIZE, stdin);
				
				if(!strncmp(sendBuffer, "quit", 4)) {
					/* send SIGINT signal to child process */
					kill(pid, SIGINT);
					break;
				}

				/* send received data to server */
				if(send(sockfd, sendBuffer, strlen(sendBuffer), 0) != strlen(sendBuffer)) {
					perror("send failed");
					exit(1);
				}
			}
	}
	close(sockfd);
	exit(0);
}

/* This is a handler that will be execute when it recieve SIGINT signal */
void signalHandler(int signo)
{
	close(sockfd);
	exit(0);
}

