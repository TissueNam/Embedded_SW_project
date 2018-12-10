#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#include <sys/time.h>
#include <sys/epoll.h>

#include "defender.h"
#include "serverlib.h"

#define BUF_SIZE 1024
#define PLAYER	2
#define HINT 3


int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

   //epoll event variable 
    struct epoll_event event;
    struct epoll_event *events;
    int epfd;
    int event_cnt, i;
 
    // other variable, this game's JJACK KKUNG variable.
    int user_cnt=0;        //user count. we start with 2 player.
    int user[2]={0};    //user information = [ socket ]
    int game_flag=0;    // Play game or not. init flag is 0. game isn't start.
    int challenger[2]={0};    //challenger [0]=socket [1]=flag	// attacter
    int examiner[2]={0};    //examiner [0]=socket [1]=flag	// defender
    char word[BUF_SIZE];    //answer
    char init_hint[BUF_SIZE]; // intial hint
    char question[BUF_SIZE];    //challenger's word
    int hint_cnt, hang_cnt, j, iscorrect, iscontinue;
    
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    // event Memory 
    events = malloc(sizeof(struct epoll_event)*50);
    
    //server socket create, bin, listen
    init_server_socket(&serv_sock, &serv_addr, argv[1]);
    
    //accept()
    clnt_addr_size=sizeof(clnt_addr);

    //struct epoll_event event;
    //struct epoll_event *events;

    epfd=epoll_create(50);    // epoll (event registration / Max : 50) fd's size = 50
    event.events=EPOLLIN;    // event type, EPOLLIN = recv data is exitst.
    event.data.fd=serv_sock;	// we monitoring server socktet.
    // epoll fd set, method, fd, epoll_event event
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);    //event registration,sign in serv_sock
	
      while(1)
    {
        printf("server socket fd = %d\n", serv_sock);
        event_cnt=epoll_wait(epfd, events, 50, -1); // return lists, tiemout = infinite

        if(event_cnt==-1)    // Terminate if error
            error_handling("epoll_wait() error!\n");
            //break;	//+)  it is dangerous.

	// event_cnt = 1 , game start and accept client socket.
        printf("Event = %d\n", event_cnt); 
        for(i=0; i<event_cnt; i++)
        {
            if(events[i].data.fd==serv_sock) // check server socket is listening. server sockter event
            {    //accept
                clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                if(clnt_sock==-1)
                    error_handling("accept() error!");
                else
                    printf("connected client: %d\n", clnt_sock); // yes connet!
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event); // ADD the client socket fd.
		// Because we will monitor the client socket. 
		

		
                user_cnt++;
		// this hangman game need 2 player whoed attacker and defender
                if(user_cnt<=PLAYER){
                    user[user_cnt-1]=clnt_sock;
                    strcpy(message, "Welcome to the Hangman game.");
                    if(user_cnt==1){
                        strcat(message, "1\n Game will be started when players come in\n");
                    }
                    else if(user_cnt==PLAYER){
                        strcat(message, "2\nGame will be started soon..\n");
			strcpy(message, "Game will be started..\n");
			send(user[0], message, strlen(message), 0);
                        game_flag=1; // game start!!
                    }
                    send(clnt_sock, message, strlen(message), 0);
                }else{  
		    // if player is disconnect or other player is still disconnect
		    // But our game's event timeout is infinite. 
		    // this problem' create possible is little.
                    send(clnt_sock, "fail", strlen("fail"), 0);
		}

                if(game_flag==1&&user_cnt==2){    // Game start. (game flag 1 and 2 player.)
                    hint_cnt=HINT; hang_cnt=0;
                    strcpy(message, "Who is the examiner..?\n");
                    send(user[0], message, strlen(message), 0);

                    // create random number
                    srand(time(NULL));
                    examiner[0]=user[rand()%PLAYER]; // 1 = attacker/ 0 = defender
                    if(examiner[0]==user[0]){
                        challenger[0]=user[1]; // attacker
		    }else{
                        challenger[0]=user[0]; 
		    }
                    strcpy(message, "You are is Attacker.\n Attacker's turn!\n");
                    send(challenger[0], message, strlen(message), 0);
                    strcpy(message, "Please wait until the defender makes quiz\n");
                    send(challenger[0], message, strlen(message), 0);
                    strcpy(message, "\nYour turn! Ha Ha u are defender. Make the Quiz!\n");
                    send(examiner[0], message, strlen(message), 0);
                    strcpy(message, "Enter the your Quiz's word in lower-case alpahbetic characters : ");
                    send(examiner[0], message, strlen(message), 0);
                    examiner[1]=1;
              	    }
            }
	     else // connected and game running. client socket event.
            {    //client socket.. receive
                str_len=recv(events[i].data.fd, message, BUF_SIZE, 0);

                if(str_len==0)
                {    // if client go out
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL); // epoll fd delete.
                    close(events[i].data.fd); // epoll is terminate.
                    printf("closed socket: %d\n", events[i].data.fd);
                    user_cnt--;
                    if(user_cnt==1 && game_flag==1){ // if there is only 1 player, change game_flag. Say 'game is over'
                        strcpy(message, "\nClient goes out, So game is over.\n");
                        challenger[1]=0;
                        examiner[1]=0;
                        if(user[0]==events[i].data.fd){
                            strcat(message, "Player 1\n");
                            strcat(message, "Game will be started when players come in\n");
                            user[0]=user[1];
                            send(user[1], message, strlen(message), 0);
                        }
                        else{
                            send(user[0], message, strlen(message), 0);
                        }
                        game_flag=0;
                    }
                }
                else if(events[i].data.fd==challenger[0] && game_flag==1)
                {
                    message[str_len-1]=0;
                    if(strcmp(message, ""))
                   if(challenger[1] == 1){
			   // Hint. (defender action.)
                            if(!strcmp(message, "hint")){    // if client wanna hint
                                if(hint_cnt==0){    //if there is no chance. HINT chance is Only 3 times...
                                    strcpy(message, "You have no chance any more\n Enter 1 alpahabetic character: ");
                                    send(events[i].data.fd, message, strlen(message), 0);
                                }
                                else{
				    strcpy(message, "Player wanna know hint.\n ");
				    strcat(message, "Enter your hint : ");
                                    send(examiner[0], message, strlen(message), 0);
                                    examiner[1]=4;
                                    challenger[1]=0;
                                }
                                break;
                            }


                            iscontinue=0; iscorrect=0;
                            if(strlen(message)==1){ //check player's answer Only 1.
                                for(j=0;  j<strlen(word); j++){
                                    if(word[j]==message[0]){
                                        iscorrect=1;
                                        question[j]=word[j];
                                    }
                                    else if(question[j]=='*')
                                        iscontinue=1;
                                }
                                strcat(message, "\n");
                                send(examiner[0], "Enter player's answer : ", strlen("Enter player's answer : "), 0);
                                send(examiner[0], message, strlen(message), 0);

                                if(iscorrect==1)    //if it's collect
                                    strcpy(message, "Your answer is collect\n");
                                else{            //if it's wrong
                                    strcpy(message, "Your answer is wrong!\n");
                                    hang_cnt++;
                                }    //notice result
                                strcat(message, "- Quiz : ");
                                strcat(message, question); strcat(message, "\ninitial HINT : ");
				strcat(message, init_hint); strcat(message, "\n");
                                strcat(message, drawHangman(hang_cnt));
                                send(examiner[0], message, strlen(message), 0);
                                send(challenger[0], message, strlen(message), 0);
                                if(hang_cnt==6){    //if it's the last chance
                                    strcpy(message, "This is your last chance.\n");
                                    send(examiner[0], message, strlen(message), 0);
                                    send(challenger[0], message, strlen(message), 0);
                                }
                                if(iscontinue==0){    //if player give all collect character
                                    strcpy(message, "Congraturaion!\nPlayer win!\n");
                                }
                                else if(hang_cnt==7){    //if player spend all chance
                                    strcpy(message, "R.I.P\nThe examiner win!\n");
                                }
                                else{    //wait player's answer again..
                                    strcpy(message, "Players are thinking..\n");
                                    send(examiner[0], message, strlen(message), 0);
                                    strcpy(message, "* 'Enter 'hint' if you wanna know hint.\n Enter lower case alpahbetic character : ");
                                    send(challenger[0], message, strlen(message), 0);
                                    break;
                                }    //Game over (if client don't go out, game will be start again)
                                strcat(message, "Game is over.\n");
                                strcat(message, "Role will be changed in next game.\nWho is the examiner in next game..?\n");
                                send(examiner[0], message, strlen(message), 0);
                                send(challenger[0], message, strlen(message), 0);
                                //Game restart
                                hint_cnt=HINT; hang_cnt=0;
                                challenger[0]=examiner[0];
                                examiner[0]=events[i].data.fd;
                                strcpy(message, "Player's turn!\n");
                                strcat(message, "Please wait until the defender makes quiz\n");
                                send(challenger[0], message, strlen(message), 0);
                                strcpy(message, "Your turn!\n");
                                strcat(message, "Enter the your word in lower-case alphabetic characters : ");
                                send(examiner[0], message, strlen(message), 0);
                                examiner[1]=1;
                                challenger[1]=0;
                                break;
                            }else{ break;}
                    }
                }
		  else if(events[i].data.fd==examiner[0] && game_flag==1)
                {
                    message[str_len-1]=0;
                    if(strcmp(message, ""))
                    switch(examiner[1]){
                        case 1:
                            
			    strcpy(word, message);
                            printf("%s\n",word);
                            strcpy(message, "**Quiz : ");
                            strcat(message, word);
			    strcat(message, "\nIntial Hint plz.(category or hint, 'spyderman' is 'hero') : ");
			    send(events[i].data.fd, message, strlen(message), 0);
				examiner[1]=2;
			    
			    break;
			case 2:
			    strcpy(init_hint, message);
			    char str_init_hint[BUF_SIZE] = "** Init hint : "; strcat(str_init_hint, message); strcat(str_init_hint, "\n");
			    send(examiner[0], str_init_hint, strlen(str_init_hint), 0);
			    send(events[i].data.fd, "\nif it is collect .. Enter 'y' / or not Enter 'n' : ", strlen("\nif it is collect .. Enter 'y' / or not Enter 'n' : " ), 0);
			    examiner[1]=3;
			    break;
                        case 3:    // check your quiz(word)
                            if(!strcmp(message, "y")){
                                examiner[1]=0;
                                challenger[1]=1;
                                for(j=0;  j<strlen(word); j++)
                                    question[j]='*';
                                question[j]='\0';

                                strcpy(message, "- Quiz : ");
                                strcat(message, question); strcat(message, "\n Init hint!! : ");
				strcat(message,init_hint); strcat(message, "\n");
                                send(examiner[0], message, strlen(message), 0);
                                send(challenger[0], message, strlen(message), 0);

                                    strcpy(message, drawHangman(0));
                                send(examiner[0], message, strlen(message), 0);
                                send(challenger[0], message, strlen(message), 0);
                                    strcpy(message, "Players are thinking.\n");
                                send(examiner[0], message, strlen(message), 0);
                                strcpy(message, "Enter lower-case alphabetic character : ");
                                send(challenger[0], message, strlen(message), 0);
                            }
                            else if(!strcmp(message, "n")){
                                strcpy(message, "Enter the your word in lower-case alphabetic characters : ");
                                send(examiner[0], message, strlen(message), 0);
                                examiner[1]=1;
                            }
                            else{
                                strcpy(message, "Please enter again.\n If it is collet.. Enter 'y' / or not Enter 'n' : ");
                                send(examiner[0], message, strlen(message), 0);
                            }
                            break;
                        case 4:    //If player wanna know hint
                            strcat(message, "\n");
                            send(challenger[0], message, strlen(message), 0);
                            hint_cnt--;
                            strcpy(message, "Players are thinking.\n");
                            send(examiner[0], message, strlen(message), 0);
                            strcpy(message, "Enter lower-case alphabetic character : ");
                            send(challenger[0], message, strlen(message), 0);
                            examiner[1]=0;
                            challenger[1]=1;
                            break;
                        default:
                        break;
                    }
                }
            }
        }
    }

    close(serv_sock);
    return 0;
}
