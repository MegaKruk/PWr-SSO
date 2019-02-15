#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#define BUFSIZE 1024 

int getMaxFD(int x, int y) 
{ 
	if (x > y) 
		return x; 
	else
		return y; 
} 
int main(int argc, char **argv) 
{ 
	int listenfd, connfd, udpfd, nready, maxfdp1, port; 
	char buff[BUFSIZE]; 
	pid_t childpid; 
	fd_set rset; 
	ssize_t n; 
	socklen_t len; 
	const int on = 1; 
	struct sockaddr_in cliaddr, servaddr; 
	void sig_chld(int); 
  
	if (argc != 2) 
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);

	//tcp
	listenfd = socket(AF_INET, SOCK_STREAM, 0); 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port); 
  
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	listen(listenfd, 10); 
  
	//udp
	udpfd = socket(AF_INET, SOCK_DGRAM, 0); 

	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
	FD_ZERO(&rset); 
  
	maxfdp1 = getMaxFD(listenfd, udpfd) + 1; 

	printf("Server online \n");
	while(1) 
	{ 
		FD_SET(listenfd, &rset); 
		FD_SET(udpfd, &rset); 
  
		nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
  
		// if tcp socket is readable then handle it by accepting the connection 
		if(FD_ISSET(listenfd, &rset)) 
		{ 
			len = sizeof(cliaddr); 
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); 
			if ((childpid = fork()) == 0) 
			{ 
				close(listenfd); 
				bzero(buff, sizeof(buff)); 
				read(connfd, buff, sizeof(buff)); 
				printf("\nMessage From TCP client: %s", buff);
				write(connfd, &buff, sizeof(buff)); 
				//sendto(udpfd, &buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
				//close(connfd); 
			}
			else 
				close(connfd); 
		} 
		// if udp socket is readable receive the message. 
		if(FD_ISSET(udpfd, &rset)) 
		{ 
			len = sizeof(cliaddr); 
			bzero(buff, sizeof(buff)); 
			n = recvfrom(udpfd, buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, &len); 
			printf("\nMessage from UDP client: %s", buff); 
			sendto(udpfd, &buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
			//write(connfd, &buff, sizeof(buff));
		} 

	} 
	close(connfd);
} 
