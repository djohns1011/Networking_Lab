#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

void main(){

    int server_sock, acpt_sock, ret_val, i=-1;

    char msg[50] = "Received Frame", read_buff[50], write_buff[50];

    fd_set set;         // to place sockets into a set for various purposes

    struct sockaddr_in serv_addr, other_addr;

    struct timeval timeout;

    socklen_t len;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&other_addr, 0, sizeof(other_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7004);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        printf("Binding Failed\n");
        exit(0);
    }

    listen(server_sock, 5);
    len = sizeof(other_addr);

    acpt_sock = accept(server_sock, (struct sockaddr*)&other_addr, &len);

    zero:
    i = i+1;
    memset(&write_buff, 0, sizeof(write_buff));

    strcpy(write_buff, msg);
    write_buff[strlen(msg)] = i + '0';

    printf("To Receive -> Frame %d", i);
    write(acpt_sock, write_buff, sizeof(write_buff));

    i = i+1;
    sleep(1);

    one:
    memset(&write_buff,0,sizeof(write_buff));

    strcpy(write_buff,msg);
    write_buff[strlen(msg)] = i + '0';
    
    printf("To Receive -> Frame %d",i);
    write(acpt_sock, write_buff, sizeof(write_buff));

    FD_ZERO(&set);
    
    FD_SET(acpt_sock, &set);

    timeout.tv_sec = 2;
    ret_val = select(acpt_sock + 1, &set, NULL, NULL, &timeout);

    if(ret_val == -1){
        printf("Error in select()");
    } else if (ret_val == 0){
        printf("---- TIMER OUT - PACKET %d SENT LOST ----",i-1);
        printf("---- RESTART TIMER ---- GO BACK N ---- ---- RESENDING PACKETS ----");
        i = i - 2;
        goto zero;
    } else {
        read(acpt_sock, read_buff, sizeof(read_buff));
        if((i==6) || (i==4) || (i==1)){
            printf("From Receiver <- %s ----- CUMULATIVE ACKNOWLEDGEMENT -----", read_buff);
        } else{
            printf("Receive <- %s ----- INDIVIDUAL ACKNOWLEDGEMENT -----", read_buff);
        }

        printf("---------------------------------------------------\n");
        i++;
        if(i>7) exit(0);
        if((i==5) || (i==3)){
            i--; goto zero;
        }
        if((i<8) || (i!=5)) goto one;
    }
    close(acpt_sock);
    close(server_sock);
}