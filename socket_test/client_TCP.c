#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7777
#define BUFSIZE 1024

int main(int argc, char* argv[])
{

	int sockfd;
	struct sockaddr_in servAddr;
	char sendBuffer[BUFSIZE];
	char recvBuffer[BUFSIZE];
	int recvLen;

	sockfd=socket(AF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(PORT);

	connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));

	while(1)
	{
		printf("Input sending msg: ");
		fgets(sendBuffer, BUFSIZE, stdin);

		if(!strncmp(sendBuffer, "quit", 4)) break;

		send(sockfd, sendBuffer, strlen(sendBuffer), 0);

		recv(sockfd, recvBuffer, BUFSIZE-1, 0);

		recvBuffer[recvLen] = '\0';
		printf("OK! %s\n", recvBuffer);
	}

	close(sockfd);
	exit(0);
	return 0;
}
