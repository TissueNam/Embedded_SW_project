 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio_ext.h>
#include <pthread.h>
 
 
 
 
 
 
int printMenu();// 메뉴 출력함수
void server(); // 서버
void client(); // 클라이언트 모드
 
void* recieve( void* p); // 메시지 읽는 쓰래드
 
void exitchat(char* str); // 종료 명령어 판별
 
 
char name[30];
 
 
int main(void)
{
    system("clear");    //콘솔 지우기
    int menu_select=printMenu(); // 메뉴 출력
 
 
 
   
    if(menu_select == 1)
    {
        printf("server 실행 ");
        server();   
   
    }
    if( menu_select ==2)
    {
        printf("클라이언트 실행 ");
        client();
    }
    else
        printf("종료합니다");
   
 
 
 
 
    return 0;
}
 
void exitchat(char* str) //  /exit를 입력 받으면 종료를 한다
{
    if(strchr(str,': /exit'))
    {
        printf("접속을 종료합니다. ");
        exit(1);
    }
}
 
 
void* recieve(void* p) //메시지 읽기 쓰래드
{
    char temp[100];
    int str_len;
    char result[150];
 
 
    while(1)
    {
           
        str_len=read((int)p,temp, sizeof(temp));
        if(str_len==0)// 접속 끊김 확인
        {
            printf("접속이 끊겼습니다. ");   
            exit(1);
 
        }
        else{
   
 
        temp[str_len]=0;
       
        exitchat(temp);// /exit 명령어가 들어왔나 안들어왔나?
 
        printf("%s ",temp);
//    printf("%d ",(int)p);   
 
        strcpy(temp,""); // 초기화
        }
    }
 
 
}
void server()
{
    int serv_sock;
    int clnt_sock;
    int port;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
 
    socklen_t clnt_addr_size;
    char message[1000];
    char result[1200];
    pthread_t r;
    char temp[100];
   
    void* s;
 
 
 
 
    printf("사용할 포트를 입력하세요 >>>");
    scanf("%d", &port); 
 
 
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1)
    {
        printf("error!!!!!: 소켓에러 ");
        exit(1);
    }
 
    memset(&serv_addr, 0, sizeof(serv_addr));
 
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(port);
   
    if(bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr))==-1)
    {
        printf("Error!: 바인드 에러 "); exit(1);
    }
 
    if(listen(serv_sock, 5) == -1)
    {
        printf("error!!: listen 에러!!! "); exit(1);
    }
 
   
   
       
   
    printf("클라이언트 기다리는중........ ");
    clnt_addr_size=sizeof(clnt_addr);
   
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
   
 
    if(clnt_sock== -1)
    {
        printf(" accept() error "); exit(1);
    }
    else
    {
        printf("클라이언트가 접속하였습니다. ");
    }
 
   
 
    __fpurge(stdin);
   
 
 
 
    pthread_create(&r,NULL,recieve,(void*)clnt_sock);// 읽기 쓰래드 시작
   
    while(1)
    {
        __fpurge(stdin);
        gets(message); // 메시지 입력 받기
        __fpurge(stdin);
        sprintf(result,"%s: %s",name,message); //닉네임과 합치기
        write(clnt_sock,result,strlen(result)); // 전송!
        exitchat(result);
   
    }
   
   
    pthread_join(r,&s);
   
    close(clnt_sock);
   
    close(serv_sock);
 
 
 
 
 
 
}
void client()
{
    int sock;
    struct sockaddr_in serv_addr;
    void* r;  // 쓰레드 관련
    pthread_t re; //쓰레드 관련
    char message[1000];
    int str_len;
       
    char ip[16];
    int port;  //포트
   
   
    printf("ip주소를 입력해주세요!!!! >>>> ");
    scanf("%s",ip);
   
    printf("포트번호를 입력해 주세요!!!>>>> ");
 
    scanf("%d",&port);
    printf("%s : %d 의 서버에 접속합니다 ", ip, port);
   
    sock=socket(PF_INET, SOCK_STREAM, 0);
   
 
    if(sock== -1)
    {
        printf("error!!!!!!! 소켓에러 ");
        exit(1);
    }
 
    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    serv_addr.sin_port=htons(port);
   
//  char temp[1000];
//  char t[100]="";
//
    char result[1200];//최종 메시지 변수
 
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
    {
       
        printf("error!!!! 컨넥트 에러!!!! "); exit(1);
    }
    //__fpurge(stdin);
   
    pthread_create(&re,NULL, recieve,(void*)sock); //read thread 생성
   
    while(1)
    {
        __fpurge(stdin);
 
   
        gets(message); //메시지 입력받음
 
 
        __fpurge(stdin);
   
        sprintf(result, "%s: %s",name,message); // 닉네임과 메시지 합치기
   
        write(sock,result,strlen(result));
 
        exitchat(result);
   
   
    }
    pthread_join(re,&r);
    close(sock);
   
 
 
 
 
 
}
 
 
 
int printMenu()//메뉴출력
{
    int select;
    printf("이름이 뭐니???>>>>>");
    scanf("%s", name);//닉네임 입력받기
   
    printf("에코클라이언트서버 ");
    printf("1. 서버실행 ");
    printf("2. 클라이언트 실행 ");
    printf("3. 종료 ");
   
    scanf("%d",&select);
 
    if(select >=1&& select <=3)
    {
        return select;
 
    }
 
    else
    {
 
        printf("잘못입력 했음 ");
        exit(1);
    }
}
  
