#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int myglobal;

void *thread_function(void *arg)
{
	int i, j;

	for (i = 0; i < 10; i++)
	{
		j = myglobal;
		j = j + 1;
		printf(".");
		fflush(stdout);
		sleep(1);
		myglobal = j;
		printf("\nmyglobal %d\n", myglobal);
	}
	return;
}

int main(void)
{
	pthread_t mythread;

	int i;

	printf("myglobal value is %d\n", myglobal);

	if (pthread_create(&mythread, NULL, thread_function, NULL))
	{
		printf("error creating thread\n");
		abort();
	}

	for (i = 0; i < 10; i++)
	{
		myglobal = myglobal + 1;
		printf("O");
		printf("\nmyglobal %d\n", myglobal);
		fflush(stdout);
		sleep(1);
	}

	if (pthread_join(mythread, NULL))
	{
		printf("error joining thread.\n");
		abort();
	}
	printf("\nmyglobal equals %d\n", myglobal);
	exit(0);
}
