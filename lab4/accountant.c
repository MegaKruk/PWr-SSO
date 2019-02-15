#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

// wplaty wypłaty z tablicy 2 kont. Odwołanie z indeksem

int main()
{
	int shmID, shmID2, res, res2, id, id2;
	int *accounts[2];
	struct sembuf g_lock_sembuf[1];
	struct sembuf g_unlock_sembuf[1];
	g_lock_sembuf[0].sem_num = 0;
	g_lock_sembuf[0].sem_op	= -1;
	g_lock_sembuf[0].sem_flg = 0;
	g_unlock_sembuf[0].sem_num = 0;
	g_unlock_sembuf[0].sem_op = 1;
	g_unlock_sembuf[0].sem_flg = 0;

	id = semget(66666, 1, IPC_CREAT|0600);
	if(id < 0)
	{
		printf("\nsemget error\n");
		exit(1);
	}
	semop(id, g_unlock_sembuf, 1);

	id2 = semget(77666, 1, IPC_CREAT|0600);
	if(id2 < 0)
	{
		printf("\nsemget2 error\n");
		exit(1);
	}
	semop(id2, g_unlock_sembuf, 1);

	shmID = shmget(66666, sizeof(int), IPC_CREAT|0600);
	if (shmID == -1)
	{
		printf("\nshmget error\n");
		exit(1);
	}

	accounts[0] = (int*)shmat(shmID, NULL, 0);
	if (accounts[0] == NULL)
	{
		printf("\nshmat error\n");
		exit(1);
	}

	shmID2 = shmget(77666, sizeof(int), IPC_CREAT|0600);
	if (shmID2 == -1)
	{
		printf("\nshmget2 error\n");
		exit(1);
	}

	accounts[1] = (int*)shmat(shmID2, NULL, 0);
	if (accounts[1] == NULL)
	{
		printf("\nshmat2 error\n");
		exit(1);
	}
	
	while(1)
	{
		while((res = semop(id, g_lock_sembuf, 1 )), res != 0) 
		{
			printf("\nsemop waiting\n");
			usleep(10000);
		}

		if(res != 0) 
		{
			printf("\nsemop error\n");
			exit(1);
		}
		else if(res == 0)
		{
			int tmp = *accounts[0];
			usleep(450000);
			*accounts[0] = tmp + 1;
			printf("\ndodano 1 do konta[0]");
			semop(id, g_unlock_sembuf, 1);
		}
		else
			printf("\nres > 0");




		while((res2 = semop(id2, g_lock_sembuf, 1 )), res2 != 0) 
		{
			printf("\nsemop2 waiting\n");
			usleep(10000);
		}

		if(res2 != 0) 
		{
			printf("\nsemop2 error\n");
			exit(1);
		}
		else if(res2 == 0)
		{
			int tmp2 = *accounts[1];
			usleep(450000);
			*accounts[1] = tmp2 + 1;
			printf("\ndodano 1 do konta[1]");
			semop(id2, g_unlock_sembuf, 1);
		}
		else
			printf("\nres2 > 0");
	}
}