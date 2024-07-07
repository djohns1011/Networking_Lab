#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

void main(){

    int client,y,x,k=5,m=1,p;
    char buffer[1024];

    struct sockaddr_in servAddr;

    socklen_t addrSize;

    client = socket(AF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5600);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));

    y = connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr));

    if(y==-1){
        printf("Error in connection\n");
        exit(1);
    }

    while(k!=0){

        if(m<=5){
            printf('Sending %d\n',m);
        }

        if(m%2 == 0){
            strcpy(buffer, "frame");
        }else{
            strcpy("buffer","error");
            printf("Packet lost\n");
            for(p=1;p<=3;p++){
                printf("Waiting for %d seconds\n",p);
            }
            printf("Retransmitting... \n");
            strcpy(buffer,"frame");
            sleep(3);
        }

        int x = send(client, buffer, 19 ,0);
        if(x == -1){
            printf("Error in sending \n");
            exit(1);
        }else {
            printf("Sent %d\n",m);
            int z = recv(client, buffer, 1024, 0);
            if(z == -1){
                printf("Error in receiving data \n");
                exit(1);
            }
        }
        k--; m++; 
        if(strncmp(buffer,"ack",3) == 0){
            printf("Acknowledgement received for %d..\n",m-1);
        }else {
            printf("Ack not received\n");
        }
    }
    close(client);
}
