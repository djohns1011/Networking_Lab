#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>

void main(){
    int sock_fd,client_sock_fd;
    struct sockaddr_in serveraddr,clientaddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5000);
    sock_fd = socket(AF_INET,SOCK_STREAM,0);

    if (sock_fd < 0){
        printf("Failed to create socket");
        return;
    }
    bind(sock_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(sock_fd,5);
    client_sock_fd = accept(sock_fd,NULL,NULL);
    if (client_sock_fd > 0){
        printf("Connected \n");
        char buf[4096];
        recv(client_sock_fd,&buf,sizeof(buf),0);
        printf("\nSending %s\n",buf);
        printf("%d",sizeof(buf));
        FILE *f = fopen(buf,"r");
        while(fgets(buf,4096,f)){
            send(client_sock_fd,buf,sizeof(buf),0);
        }
    }
}