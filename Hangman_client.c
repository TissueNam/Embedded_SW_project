#include <stdio.h>
#include <string.h>
#include <errno.h>
#include  <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int socket;
	char msg[BUF_SIZE];
	int str_len;
	int rcv_len;
	int rcv_cnt;
	struct sockaddr_in serv_addr;
	pthread_t send_thread;
	pthread_t rcv_thread;
	void *th_return;

	// parameter ./hangman_client server_addr(IP) server_PORT
	if(argc < 3){ printf("uncorrect exe command ./cllient server_IP server_PORT\n");	exit(-1);}

	// IPv4 TCP
	sock = socket(PV_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock != 0){  perror("socket() error"); exit(-2); }
	
	// sockaddr_in init
	memset(&serv_addr, 0 sizeof(serv_addr));
	serv_addr.sin_family = AD_INET; // IPv4
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // Input server IP address
	serv_addr.sin_port=htons(atoi(argv[2])); // PORT


	return 0;
}
