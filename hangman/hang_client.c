#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 1024

void* send_msg(void* data);
void* recv_msg(void* data);

int sockfd;
int main(int argc, char* argv[]){
	
	struct sockaddr_in serv_addr;
	char send_buffer[BUF_SIZE], recv_buffer[BUF_SIZE];
	int recv_len, recv_cnt, str_len;
	pid_t pid;
	pthread_t send_thread, recv_thread;
	void* thread_return;
	
	if(argc < 3){
		fprintf(stderr, "Usage: %s IP_address, %s server_PORT", argv[1], argv[2]);
		exit(0);
	}

	// create socket IPv4 , TCP/IP, TCP refenrence
	if((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))== -1){
		perror("socket() failed");
		exit(1);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// connect
	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))== -1){
		perror("connect() failed");
		exit(1);
	}else{
		printf("Connect!!\n");
	}

	pthread_create(&send_thread, NULL, send_msg, (void*)&sockfd);
	pthread_create(&recv_thread, NULL, recv_msg, (void*)&sockfd);

	pthread_join(send_thread, &thread_return);
	pthread_join(recv_thread, &thread_return);

	close(sockfd);

	return 0;
}

void *send_msg(void* data)
{
	int sockfd = *((int*)data);
	int str_len;
	char * result;
	char msg[BUF_SIZE];
	while(1)
	{
		if((result = fgets(msg, BUF_SIZE, stdin)) != NULL)
		{
			//printf
		}
		str_len = write(sockfd, msg, strlen(msg)+1);
	
	}
}

void *recv_msg(void* data)
{
	int sockfd = *((int*)data);
	int recv_cnt;
	char msg[BUF_SIZE];

	while(1)
	{
		recv_cnt = read(sockfd, &msg ,BUF_SIZE);
		if(recv_cnt == -1)
			printf("read() error");

		msg[BUF_SIZE] = 0;
		if(recv_cnt != 0)
		{
			printf("clinet recve msg: %s", msg);
		}
	}
}
