#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{ 
	int pid, status; 

	if((pid = fork()) == 0) 
	{
		printf("Proces potomny utworzony\n");
		sleep(2);
		exit(1);
	}  

	pid = wait(&status);
	printf("Proces %d zakończony, status %d\n", pid, WEXITSTATUS(status)); 

	exit(0);
	return 0; 
}