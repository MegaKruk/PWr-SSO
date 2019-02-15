#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[])
{
	//char *cmd1[] = { argv[1], argv[2] }; 
	//char *cmd2[] = { argv[3], argv[4] };

	int fd[2], pid;
	
	pipe(fd);
	
	pid = fork();

	if(pid < 0) 
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{	
		close(fd[0]);

		close(1);

		dup(fd[1]);

		close(fd[1]);

		execlp(argv[1], argv[1], argv[2], NULL);
	}
	else
	{
		close(fd[1]);

		close(0);

		dup(fd[0]);

		close(fd[0]);

		execlp(argv[3], argv[3], argv[4], NULL);

		return 0;
	}
}