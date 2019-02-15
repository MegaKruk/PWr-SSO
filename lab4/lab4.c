#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

// wplaty wypłaty z tablicy 2 kont. Odwołanie z indeksem

int main()
{
	int shmID, shmID2;
	int *accounts[2];

	shmID = shmget(66666, sizeof(int), IPC_CREAT|0600);
	if (shmID == -1)
	{
		perror("\nshmget error\n");
		exit(1);
	}
	else
		printf("\nutworzono segment pamięci współdzielonej o id %d\n", shmID);

	shmID2 = shmget(77666, sizeof(int), IPC_CREAT|0600);
	if (shmID2 == -1)
	{
		perror("shmget2 error");
		exit(1);
	}
	else
		printf("\nutworzono segment pamięci współdzielonej o id %d\n", shmID2);


	accounts[0] = (int*)shmat(shmID, NULL, 0);
	if (accounts[0] == NULL)
	{
		perror("\nshmat error\n");
		exit(1);
	}
	else
		printf("\nprzyłączono segment pamięci współdzielonej o id %d\n", shmID);

	accounts[1] = (int*)shmat(shmID2, NULL, 0);
	if (accounts[1] == NULL)
	{
		perror("shmat error");
		exit(1);
	}
	else
		printf("\nprzyłączono segment pamięci współdzielonej o id %d\n", shmID2);



	while(1)
	{
		printf("\nkonto[0] = %d", *accounts[0]);
		printf("\nkonto[1] = %d\n", *accounts[1]);
		usleep(1000000);
	}
}