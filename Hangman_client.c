#include <stdio.h>
#include <string.h>
#include <errno.h>
#include  <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

void *send_msg(void* arg)
{
	int sock = *((int*)arg);
	int str_len;
	char msg[BUF_SIZE];
	while(1)
	{
		fgets(msg, BUF_SIZE, stdin);
		str_len = write(sock, msg, strlen(msg));
	}
}

void *rcv_msg(void* arg)
{
	int sock = *((int*)arg);
	int rcv_cnt;
	char msg[BUF_SIZE];

	while(1)
	{	// message read
		rcv_cnt = read(sock, &msg, BUF_SIZE-1);
		if(rcv_cnt == -1) { perror("read error"); exit(1); }

		msg[rcv_cnt] = 0; // EOF
		if(!strcmp(msg, "fail")) // room is full or server is exit.
			printf("asdasdasdsad hcw sex");

		printf("%s",msg);
	}
}

int main(int argc, char *argv[])
{
	int sock;
	char msg[BUF_SIZE];
	int str_len;
	int rcv_len;
	int rcv_cnt;
	struct sockaddr_in serv_addr;
	pthread_t send_thread;
	pthread_t rcv_thread;
	void *th_return;

	// parameter ./hangman_client server_addr(IP) server_PORT
	if(argc < 3){ printf("uncorrect exe command ./cllient server_IP server_PORT\n");	exit(1);}

	// IPv4 TCP
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock != 0){  perror("socket() error"); exit(1); }
	
	// sockaddr_in init
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; // IPv4
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // Input server IP address
	serv_addr.sin_port=htons(atoi(argv[2])); // PORT

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))== -1)
	{
	       	perror("connect error"); 
		exit(1);
	}else{
		printf("Connected!!\n");
	}
	
	pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, rcv_msg, (void*)&sock);

	pthread_join(send_thread, &th_return);
	pthread_join(rcv_thread, &th_return);

	close(sock);	
	return 0;
}
