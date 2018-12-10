#include <stdio.h>
#include <string.h>
#include <stdlib.c>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define MAXPENDING 3
int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	char sned_msg[BUF_SIZE];
	char recv_msg[BUF_SIZE];
	int sendlen, recvlen;
	socklen_t clnt_addr_size;
	
	
	if(argc != 2)
	{
		printf("Usage : PORT <%s>\n", argv[1]);
		exit(0);
	}

	//create socket
	if((serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))==-1)
	{
		perror("socket() error");
		exit(0);
	}

	memset(&serv_addr, 0 sizeof(serv_addr));
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sieof(serv_addr))==-1)
	{
		perror("bind() error");
		exit(0);
	}

	// possible client number = 3 
	if(listen(serv_sock,MAXPENDING) == -1)
	{
		perror("listen() error");
		exit(0);
	}
	while(1)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		printf("* %s:%d want to connect\n", inet_ntoa(clnt_addr.sin_addr),ntohs(clinet_adr.sin_port));
		if(clnt_sock == -1)
		{
			perror("accept() error");
			exit(0);
		}else{
			printf("connected client: %d\n", clnt_sock.sin_addr);
		}

	}

	close(serv_sock);
	return 0;
}
