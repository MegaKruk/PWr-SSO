#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define BUFSIZE 1024 

int main(int argc, char **argv) 
{ 
	int sockfd, port; 
	char buff[BUFSIZE]; 
	struct sockaddr_in servaddr; 

	if (argc != 2) 
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);

	int n, len; 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{ 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	while(1)
	{
		// send message to server 
		printf("\nPlease enter msg: ");
		bzero(buff, BUFSIZE);
		fgets(buff, BUFSIZE, stdin);
		sendto(sockfd, &buff, strlen(buff), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr)); 

		// receive server's response 
		printf("\nResponse from server: "); 
		n = recvfrom(sockfd, (char*)buff, BUFSIZE, 0, (struct sockaddr*)&servaddr, &len); 
		puts(buff); 
	}
	close(sockfd); 
	return 0; 
} 
