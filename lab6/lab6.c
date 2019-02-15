#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define noOfAccs  8

/* 
1. wplaty wyplaty
2. wyplata. czy sa wystarczajace srodki
3. przelewy
*/

pthread_t accountant[noOfAccs];
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t waitformoney1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t waitformoney2 = PTHREAD_COND_INITIALIZER;
int account[2] = { 0 };
int ctr;
int stop = 0;
int ptr = 0;
int block1 = 1;
int blockmoney1 = 0;
int block2 = 1;
int blockmoney2 = 0;
int err;

void sendMoney(int id)
{
	int choice = rand()%2;
	if(choice == 0)
	{	
		pthread_mutex_lock(&myMutex);
		while(block1 <= 0)
		{
			pthread_cond_wait(&cond1, &myMutex); 
		}
		
		block1--;
		pthread_mutex_unlock(&myMutex);

			int tmp1 = 1;
			usleep(500000);
			account[choice] += tmp1;
			printf("\nAccountant[%d] sent %d$ to account[%d]\t\t\t| account[%d] = %d\n", id, tmp1, choice, choice, account[choice]);

		pthread_mutex_lock(&myMutex);
		if(account[choice] >= 2)
		{
			blockmoney1 = 1;
			pthread_cond_signal(&waitformoney1); 
		}
		else
			blockmoney1 = 0;
		block1++;
		pthread_cond_signal(&cond1); 
		pthread_mutex_unlock(&myMutex);
	}
	else /*******************************************/
	{
		pthread_mutex_lock(&myMutex);
		while(block2 <= 0)
		{
			pthread_cond_wait(&cond2, &myMutex); 
		}
		
		block2--;
		pthread_mutex_unlock(&myMutex);

			int tmp2 = 1;
			usleep(500000);
			account[choice] += tmp2;
			printf("\nAccountant[%d] sent %d$ to account[%d]\t\t\t| account[%d] = %d\n", id, tmp2, choice, choice, account[choice]);

		pthread_mutex_lock(&myMutex);
		if(account[choice] >= 2)
		{
			blockmoney2 = 1;
			pthread_cond_signal(&waitformoney2); 
		}
		else
			blockmoney2 = 0;
		block2++;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock(&myMutex);
	}
}

void withdrawMoney(int id)
{
	int choice = rand()%2;
	if(choice == 0)
	{	
		pthread_mutex_lock(&myMutex);
		while(block1 <= 0 || blockmoney1 <= 0)
		{
			if(blockmoney1 <= 0)
				pthread_cond_wait(&waitformoney1, &myMutex);
			else if(block1 <= 0)
				pthread_cond_wait(&cond1, &myMutex); 
		}
		
		block1--;
		blockmoney1 = 0;
		pthread_mutex_unlock(&myMutex);

			int tmp1 = 2;
			usleep(500000);
			account[choice] -= tmp1;
			printf("\nAccountant[%d] withdrew %d$ from account[%d]\t\t| account[%d] = %d\n", id, tmp1, choice, choice, account[choice]);

		pthread_mutex_lock(&myMutex);
		if(account[choice] >= 2)
		{
			blockmoney1 = 1;
			pthread_cond_signal(&waitformoney1); 
		}
		else
			blockmoney1 = 0;
		block1++;
		pthread_cond_signal(&cond1); 
		pthread_mutex_unlock(&myMutex);
	}
	else/*******************************************/
	{
		pthread_mutex_lock(&myMutex);
		while(block2 <= 0 || blockmoney2 <= 0)
		{
			if(blockmoney2 <= 0)
				pthread_cond_wait(&waitformoney2, &myMutex);
			else if(block2 <= 0)
				pthread_cond_wait(&cond2, &myMutex); 
		}
		
		block2--;
		blockmoney2 = 0;
		pthread_mutex_unlock(&myMutex);

			int tmp2 = 2;
			usleep(500000);
			account[choice] -= tmp2;
			printf("\nAccountant[%d] withdrew %d$ from account[%d]\t\t| account[%d] = %d\n", id, tmp2, choice, choice, account[choice]);

		pthread_mutex_lock(&myMutex);
		if(account[choice] >= 2)
		{
			blockmoney2 = 1;
			pthread_cond_signal(&waitformoney2); 
		}
		else
			blockmoney2 = 0;
		block2++;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock(&myMutex);
	}
}

void transction(int id)
{
	int choice = rand()%2;
	if(choice == 0)
	{	
		pthread_mutex_lock(&myMutex);
		while(block1 <= 0 || block2 <= 0 || blockmoney2 <= 0)
		{
			if(blockmoney2 <= 0)
				pthread_cond_wait(&waitformoney2, &myMutex);
			else if(block1 <= 0)
				pthread_cond_wait(&cond1, &myMutex); 
			else if(block2 <= 0)
				pthread_cond_wait(&cond2, &myMutex);
		}
		block2--;
		block1--;
		blockmoney2 = 0;
		pthread_mutex_unlock(&myMutex);

			int tmp1 = 2;
			usleep(500000);
			account[choice + 1] -= tmp1;
			account[choice] += tmp1;
			printf("\nAccountant[%d] transferred %d$ from account[%d] to [%d]\t| account[%d] = %d | account[%d] = %d\n", id, tmp1, choice + 1, choice, choice, account[choice], choice + 1, account[choice + 1]);

		pthread_mutex_lock(&myMutex);
		if(account[choice] >= 2)
		{
			blockmoney1 = 1;
			pthread_cond_signal(&waitformoney1); 
		}
		else
			blockmoney1 = 0;
		if(account[choice + 1] >= 2)
		{
			blockmoney2 = 1;
			pthread_cond_signal(&waitformoney2); 
		}
		else
			blockmoney2 = 0;
		block1++;
		pthread_cond_signal(&cond1); 
		block2++;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock(&myMutex);
	}
	else
	{
		pthread_mutex_lock(&myMutex);
		while(block1 <= 0 || block2 <= 0 || blockmoney1 <= 0)
		{
			if(blockmoney1 <= 0)
				pthread_cond_wait(&waitformoney1, &myMutex);
			else if(block1 <= 0)
				pthread_cond_wait(&cond1, &myMutex); 
			else if(block2 <= 0)
				pthread_cond_wait(&cond2, &myMutex);
		}
		block2--;
		block1--;
		blockmoney1 = 0;
		pthread_mutex_unlock(&myMutex);

			int tmp2 = 2;
			usleep(500000);
			account[choice - 1] -= tmp2;
			account[choice] += tmp2;
			printf("\nAccountant[%d] transferred %d$ from account[%d] to [%d]\t| account[%d] = %d | account[%d] = %d\n", id, tmp2, choice - 1, choice, choice - 1, account[choice - 1], choice, account[choice]);

		pthread_mutex_lock(&myMutex);
		if(account[choice - 1] >= 2)
		{
			blockmoney1 = 1;
			pthread_cond_signal(&waitformoney1); 
		}
		else
			blockmoney1 = 0;
		if(account[choice] >= 2)
		{
			blockmoney2 = 1;
			pthread_cond_signal(&waitformoney2); 
		}
		else
			blockmoney2 = 0;
		block1++;
		pthread_cond_signal(&cond1); 
		block2++;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock(&myMutex);
	}
}

void* processFunc(void *arg)
{
	int myID = ++ctr;
	printf("\nAccountant[%d] created\n", myID);
	while(!stop)
	{
		if(myID < 5)
			sendMoney(myID);
		else if(myID < 7)
			withdrawMoney(myID);
		else
			transction(myID);
		usleep(1000000);
	}
	printf("\nAccountant[%d] died\n", myID);

	return NULL;
}

int main(void)
{
	srand(time(NULL));
	while(ptr < noOfAccs)
	{
		err = pthread_create(&(accountant[ptr]), NULL, &processFunc, NULL);
		if (err != 0)
			printf("\nError: can't create thread :[%s]", strerror(err));
		ptr++;
	}

	getchar();
	stop = 1;

	for (int i = 0; i < noOfAccs; ++i)
		pthread_join(accountant[i], NULL);

	return 0;
}