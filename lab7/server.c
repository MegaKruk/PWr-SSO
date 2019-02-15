#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define BUFLEN 256

void error(char *msg) 
{
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) 
{
  int fd, newfd, port, clientlen, optval, n, recs, stop; 
  struct sockaddr_in myaddr, client_addr; 
  char buf[BUFLEN]; 
  fd_set readfds;


  if (argc != 2) 
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  port = atoi(argv[1]);
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) 
    error("ERROR opening socket");

  optval = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

  bzero((char *) &myaddr, sizeof(myaddr));

  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons((unsigned short)port);

  if (bind(fd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) 
    error("ERROR on binding");

  if (listen(fd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  clientlen = sizeof(client_addr);
  stop = 1;
  recs = 0;
  printf("server> ");
  fflush(stdout);

  while (stop) 
  {
    FD_ZERO(&readfds);          
    FD_SET(fd, &readfds); 
    FD_SET(0, &readfds);       
    if (select(fd+1, &readfds, 0, 0, 0) < 0) 
      error("ERROR in select");

    /* if the user has entered a command, process it */
    if (FD_ISSET(0, &readfds)) 
    {
      fgets(buf, BUFLEN, stdin);
        switch (buf[0]) 
        {
        case 'c': /* print the connection cnt */
        	printf("Received %d connection requests so far.\n", recs);
        	printf("server> ");
        	fflush(stdout);
        	break;
        case 'q': /* terminate the server */
        	stop = 0;
        	break;
        default: /* bad input */
        	printf("ERROR: unknown command\n");
        	printf("server> ");
        	fflush(stdout);
        }
    }    

    /* if a connection request has arrived, process it */
    if (FD_ISSET(fd, &readfds)) 
    {
      newfd = accept(fd, 
		       (struct sockaddr *) &client_addr, &clientlen);
      if (newfd < 0) 
	       error("ERROR on accept");
      recs++;
      
      bzero(buf, BUFLEN);
      n = read(newfd, buf, BUFLEN);
      if (n < 0) 
	      error("ERROR reading from socket");
      
      n = write(newfd, buf, strlen(buf));
      if (n < 0) 
		    error("ERROR writing to socket");
      
      close(newfd);
    }
  }

  /* clean up */
  printf("Terminating server.\n");
  close(fd);
  exit(0);
}