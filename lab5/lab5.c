#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

static const int noOfAccs = 3;

pthread_t accountant[noOfAccs];
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int account[2] = { 0 };
int ctr;
bool stop = false;
int ptr = 0;
int block1 = 1;
int block2 = 1;
int err;

void* processFunc(void *arg)
{
	int myID = ++ctr;
	printf("\nAccountant %d created\n", myID);
	while(!stop)
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
			account[0] += tmp1;
			printf("\nAdded 1 to account[0] | account[0] = %d\n", account[0]);

		pthread_mutex_lock(&myMutex);
		block1++;
		pthread_cond_signal(&cond1); 
		pthread_mutex_unlock(&myMutex);

		usleep(1000000);

		pthread_mutex_lock(&myMutex);
		while(block2 <= 0)
		{
			pthread_cond_wait(&cond2, &myMutex); 
		}
		block2--;
		pthread_mutex_unlock(&myMutex);

			int tmp2 = 1;
			usleep(500000);
			account[1] += tmp2;
			printf("\nAdded 1 to account[1] | account[1] = %d\n", account[1]);

		pthread_mutex_lock(&myMutex);
		block2++;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock(&myMutex);
	}
	printf("\nAccountant %d died\n", myID);

	return NULL;
}

int main(void)
{
	while(ptr < noOfAccs)
	{
		err = pthread_create(&(accountant[ptr]), NULL, &processFunc, NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		ptr++;
	}

	getchar();
	stop = true;

	for (int i = 0; i < noOfAccs; ++i)
		pthread_join(accountant[i], NULL);

	return 0;
}