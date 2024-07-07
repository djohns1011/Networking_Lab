#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 3000

typedef struct ack_buff{
	int ack;
	bool should_sent;
}ACK_BUFFER;


void main(){
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server_addr;
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	int server_len=sizeof(server_addr);
	int window_size,total_packets;

	printf("Enter the window size :");
	scanf("%d",&window_size);

	total_packets=window_size*5;

	sendto(sockfd,&window_size,sizeof(window_size),0,(struct sockaddr*)&server_addr,server_len);
	printf("Sent the window Size to the server\n");

	int i=0,j,k,x;
	ACK_BUFFER ACB[total_packets];

	while(true){
		j=i;
		k=0;

		recvfrom(sockfd,&ACB,sizeof(ACB),0,(struct sockaddr*)&server_addr,&server_len);

		while(j<total_packets && k<window_size){
			if(ACB[j].should_sent){
				printf("Enter the ACK for frame %d: ",j);
				scanf("%d",&ACB[j].ack);
				k++;
			}

			j++;
		}

		printf("\n");

		sendto(sockfd,&ACB,sizeof(ACB),0,(struct sockaddr*)&server_addr,server_len);

		recvfrom(sockfd,&x,sizeof(x),0,(struct sockaddr*)&server_addr,&server_len);

		if(x>=total_packets){
			printf("All frames successfully delivered to Client\n");
			break;
		}

		else{
			i=x;
		}
	}

}