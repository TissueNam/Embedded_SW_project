#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7777
#define MAXPENDING 5
#define BUFSIZE 1024

int main(void)
{
	int servSockfd;
	int clntSockfd;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	char recvBuffer[BUFSIZE];
	int clntLen;
	int recvLen;
	char clntIP[20];

	if((servSockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("creat socket failed");
		exit(0);
	}

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);

	if(bind(servSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr))==-1)
	{
		perror("bind failed");
		exit(0);
	}

	if(listen(servSockfd, MAXPENDING) == -1)
	{
		perror("listen failed");
		exit(0);
	}

	while(1)
	{
		clntLen = sizeof(clntAddr);

		clntSockfd=accept(servSockfd, (struct sockaddr*)&clntAddr, &clntLen);
		inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntIP, sizeof(clntIP));
		printf("sever : %s client connected. \n",clntIP);
		// client IP check.

		while(1){
			
			//if(recvLen == 0) printf("client out\n"); break;

			//recvBuffer[recvLen] = '\0';

			//printf("client->server : %s\n", recvBuffer);

			//send(clntSockfd, "server receive msg",30, 0 );
			
			recvLen = read(clntSockfd, recvBuffer, BUFSIZE);
			write(clntSockfd, recvBuffer,recvLen);

			close(clntSockfd);
		}
	}

	return 0;
}
