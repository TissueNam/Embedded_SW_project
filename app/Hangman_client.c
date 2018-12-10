#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "hanglib.h"

#define BUF_SIZE 1024

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
<<<<<<< HEAD
	if(sock == -1){  perror("socket() error"); exit(1); }
=======
	if(sock == -1){  error_handling("socket() error"); }
>>>>>>> 7f9ca9d223b50e9099ecbdb78cf0a3a53875fa4f
	
	// sockaddr_in init
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; // IPv4
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // Input server IP address
	serv_addr.sin_port=htons(atoi(argv[2])); // PORT

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))== -1)
	{
	       	error_handling("connect error");
	       perror("connect()");	
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
