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
#define IP_STRING_ADDRESS "127.0.0.1"
#define MAX 100
#define SA struct sockaddr

void tcp_connection(int connfd){
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		recv(connfd, buff, sizeof(buff),0);
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		send(connfd, buff, sizeof(buff),0);

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}


int main(int argc, char *argv[]){
	int port = atoi(argv[1]);
	int protocol;
	printf("Select protocol (0 for tcp,1 for udp):: ");
	scanf("%d", &protocol);
	if(protocol == 0){
	  int sockfd, connfd;
	  unsigned int len;
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
	  servaddr.sin_addr.s_addr = inet_addr(IP_STRING_ADDRESS);
	  servaddr.sin_port = htons(port);

	  // Binding newly created socket to given IP and verification
	  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	  }
	  else
		printf("Socket successfully binded..\n");

	  // Now server is ready to listen and verification
	  if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	  }
	  else
		printf("Server listening..\n");
	  len = sizeof(cli);

	  // Accept the data packet from client and verification
	  connfd = accept(sockfd, (SA*)&cli, &len);
	  if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	  }
	  else
		printf("server accept the client...\n");

	  // Function for chatting between client and server
	  tcp_connection(connfd);

	  // After chatting close the socket
	  close(sockfd);
	}
	else if(protocol == 1){
	  char buffer[100];
	  char *message = (char *) "Hello Client";
	  int listenfd;
	  unsigned int len;
	  struct sockaddr_in servaddr, cliaddr;
	  bzero(&servaddr, sizeof(servaddr));

	  //Create a UDP Socket
	  listenfd = socket(AF_INET, SOCK_DGRAM, 0);		
	  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  servaddr.sin_port = htons(port);
	  servaddr.sin_family = AF_INET;

	  // bind server address to socket descriptor
	  bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	  //receive the datagram
	  len = sizeof(cliaddr);
	  int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&len); //receive message from server
	  buffer[n] = '\0';

	  printf("Server : %s\n", buffer);
		
	  // send the response
	  sendto(listenfd, message, MAX, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
	}



}
