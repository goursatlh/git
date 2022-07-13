#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int myglobal;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg)
{
	int i, j;
    int k;

	for (i = 0; i < 10000; i++)
	{
		pthread_mutex_lock(&mymutex);
		j = myglobal;
		j = j + 1;
		printf(".");
		fflush(stdout);
		//sleep(1);
		usleep(1000);
		printf("^");
		fflush(stdout);
		myglobal = j;
        //if (i == 3000)
        //    k = k/0;
		pthread_mutex_unlock(&mymutex);
	}
	return NULL;
}

int main(void)
{
	pthread_t mythread;

	int i;

	if (pthread_create(&mythread, NULL, thread_function, NULL))
	{
		printf("error creating thread.\n");
		abort();
	}
	for (i = 0; i < 10000; i++)
	{

		printf("$");
		fflush(stdout);
		pthread_mutex_lock(&mymutex);
		myglobal = myglobal + 1;
		pthread_mutex_unlock(&mymutex);
		printf("O");
		fflush(stdout);
		//sleep(1);
	}
	if (pthread_join(mythread, NULL))
	{
		printf("error joining thread.\n");
		abort();
	}
	printf("\nmyglobal equals % d\n", myglobal);
	exit(0);
}
