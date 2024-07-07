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
	struct sockaddr_in server_addr,client_addr;
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	int client_len=sizeof(client_addr);
	bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

	int window_size,total_packets;

	recvfrom(sockfd,&window_size,sizeof(window_size),0,(struct sockaddr*)&client_addr,&client_len);
	printf("Received the window size %d from the client\n",window_size);

	total_packets=window_size*5;
	printf("The total number of packets will be %d\n",total_packets);

	int i,j,k;
	bool negative;
	ACK_BUFFER ACB[total_packets];

	for(i=0;i<total_packets;i++){
		ACB[i].should_sent=true;
	}

	i=0;

	while(true){
		j=i;
		k=0;
		negative=false;

		while(j<total_packets && k<window_size){
			if(ACB[j].should_sent){
				printf("Frame to be transmitted %d\n",j);
				k++;
			}

			j++;
		}

		printf("\n");

		sendto(sockfd,&ACB,sizeof(ACB),0,(struct sockaddr*)&client_addr,&client_len);

		recvfrom(sockfd,&ACB,sizeof(ACB),0,(struct sockaddr*)&client_addr,&client_len);

		j=i;
		k=0;

		while(j<total_packets && k<window_size){
			if(ACB[j].ack==-1 && ACB[j].should_sent){
				printf("Negative ACK received for frame %d\n",j);

				if(!negative){
					negative=true;
					i=j;
				}
				k++;
			
			}

			else if(ACB[j].should_sent){
				ACB[j].should_sent=false;
				k++;
			}

			j++;
		}

		printf("\n");

		if(!negative){
			printf("\nAll frames in windows delivered\n");
			i+=window_size;
		}

		sendto(sockfd,&i,sizeof(i),0,(struct sockaddr*)&client_addr,client_len);

		if(i>=total_packets){
			printf("\nAll frames delivered successfully\n");
			break;
		}
	}

}