#include "hanglib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 1024

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void *rcv_msg(void* data)
{
	int sock= *((int*)data);
	int recv_cnt;
	char msg[BUF_SIZE];

	while(1)
	{
		if((recv_cnt = read(sock, &msg, BUF_SIZE-1)) == -1) 
		       	perror("read error");
		msg[recv_cnt] = 0;
		if(!strcmp(msg, "fail"))
			printf("Sorry, Server is full or closed now, You can't access\n");
		
		printf("%s", msg);	
	}
}

void *send_msg(void* data)
{
	int sock = *((int*)data);
	int str_len;
	char msg[BUF_SIZE];

	while(1)
	{
		fgets(msg, BUF_SIZE, stdin);
		str_len = write(sock, msg, strlen(msg));
	}
}
