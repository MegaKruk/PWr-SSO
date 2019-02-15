#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define BUFSIZE 1024 

int main(int argc, char **argv) 
{ 
	int sockfd, port, n, len; 
	char buff[BUFSIZE]; 
	struct sockaddr_in servaddr;  

	if (argc != 2) 
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);

	while(1)
	{
		// Creating socket file descriptor 
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{ 
			printf("socket creation failed"); 
			exit(0); 
		} 
	  
		memset(&servaddr, 0, sizeof(servaddr)); 
	  
		// Filling server information 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(port); 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	  
		if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
			printf("\n Error : Connect Failed \n");  
	  
		memset(buff, 0, sizeof(buff)); 
		printf("\nPlease enter msg: ");
		bzero(buff, BUFSIZE);
		fgets(buff, BUFSIZE, stdin);
		write(sockfd, buff, sizeof(buff)); 
		printf("\nResponse from server: "); 
		read(sockfd, buff, sizeof(buff)); 
		puts(buff); 
		close(sockfd); 
	} 
}
