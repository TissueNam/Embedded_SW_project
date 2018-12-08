//#ifdef __HANGLIB_H__
//#define __HANGLIB_H__

#define BUF_SIZE 1024

void error_handling(char *msg);
void* rcv_msg(void* data);
void* send_msg(void* data);

//#endif
