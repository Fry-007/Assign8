#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 100
#define SA struct sockaddr

void tcp_connection(int sockfd){
  	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		send(sockfd, buff, sizeof(buff),0);
		bzero(buff, sizeof(buff));
		recv(sockfd, buff, sizeof(buff),0);
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main(int argc, char *argv[]){
	int port = atoi(argv[2]);
	int protocol;
	char *ipAdd = argv[1];
	printf("Select the protocol(0 for tcp, 1 for udp):: ");
	scanf("%d\n",protocol);
	if(protocol == 0){
	  int sockfd, connfd;
	  struct sockaddr_in servaddr, cli;

	  // socket create and verification
	  sockfd = socket(AF_INET, SOCK_STREAM, 0);
	  if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	  }
	  else
		printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(ipAdd);
		servaddr.sin_port = htons(port);

		// connect the client socket to server socket
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
		}
		else
		  printf("connected to the server..\n");
	
		// function for chat
		tcp_connection(sockfd);

		// close the socket
		close(sockfd);
	  }
	  else if(protocol == 1){
		char buffer[100];
		char *message = (char *) "Hello Server";
		int sockfd, n;
		unsigned int len;
		struct sockaddr_in servaddr;
	
		// clear servaddr
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_addr.s_addr = inet_addr(ipAdd);
		servaddr.sin_port = htons(port);
		servaddr.sin_family = AF_INET;
	
		// create datagram socket
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
		if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		{
	   	 printf("\n Error : Connect Failed \n");
	   	 exit(0);
		 }

		// request to send datagram
		// no need to specify server address in sendto
			// connect stores the peers IP and port
		sendto(sockfd, message, MAX, 0, (struct sockaddr*)&servaddr, 			sizeof(servaddr));
	
		// waiting for response
		n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, 	&len);
        
		buffer[n] = '\0';
        	printf("Server : %s\n", buffer);
		//close the descriptor
		close(sockfd);
	  }





}
