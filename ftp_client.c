#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

int main(){
    int sock_fd;
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5000);
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if (sock_fd < 0){
        printf("Failed to create socket");
        return 0;
    }
    if (connect(sock_fd,((struct sockaddr*) &serveraddr),sizeof(serveraddr)) != -1){
        printf("Enter file name to fetch from server: ");
        char filename[100];
        scanf("%s",filename);
        send(sock_fd,filename,sizeof(filename),0);
        char buf[4096];
        printf("\n");
        while (recv(sock_fd,buf,sizeof(buf),0) != 0) {
            printf("%s",buf);
        }   
    }else{
        printf("Failed to connect");
    }

    return 0;
}