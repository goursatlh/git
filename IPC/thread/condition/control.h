#include <pthread.h>

typedef struct data_control
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int active;
} data_control;

pthread_cond_t mycond;

#if 0
pthread_cond_init(&mycond, NULL);
pthread_cond_destroy(&mycond);
pthread_cond_wait(&mycond, &mymutex);
pthread_cond_broadcast(&mycond);
pthread_cond_signal(&mycond);
#endif
