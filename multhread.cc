#if 0 // mutex
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int myglobal;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg)
{
    int i,j;
    for ( i=0; i<20; i++ ) {
        pthread_mutex_lock(&mymutex);
        j=myglobal;
        j=j+1;
        printf(".");
        fflush(stdout);
        sleep(1);
        myglobal=j;
        pthread_mutex_unlock(&mymutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t mythread;
    int i;

    if ( pthread_create( &mythread, NULL, thread_function, NULL) ) {
        printf("error creating thread.");
        abort();
    }

    for ( i=0; i<20; i++) {
        pthread_mutex_lock(&mymutex);
        myglobal=myglobal+1;
        pthread_mutex_unlock(&mymutex);
        printf("o");
        fflush(stdout);
        sleep(1);
    }

    if ( pthread_join ( mythread, NULL ) ) {
        printf("error joining thread.");
        abort();
    }

    printf("\nmyglobal equals %d\n",myglobal);
    exit(0);
}
#endif

#if 1 // condition
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread1(void *);
void *thread2(void *);

int i = 1;
int main(void)
{
    pthread_t t_a;
    pthread_t t_b;

    pthread_create(&t_a, NULL, thread2, (void *)NULL);
    pthread_create(&t_b, NULL, thread1, (void *)NULL);
    pthread_join(t_b, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}


void *thread1(void *junk)
{       
    while (i < 30)
    {       
        pthread_mutex_lock(&mutex);
        if (i % 3 == 0)
        {       
            pthread_cond_signal(&cond);    // cond is (i % 3 == 0), true wake up thread2, then i sleep 
            pthread_cond_wait(&cond, &mutex);
        }
        printf("thead1:%d\n", i);
        ++i;
        pthread_mutex_unlock(&mutex);
        //sleep(1);
    }
}

void *thread2(void *junk)
{
    while (i < 30)
    {
        pthread_mutex_lock(&mutex);
        if (i % 3 != 0)
        {
            pthread_cond_signal(&cond);// cond is (i % 3 != 0), true wake up thread1, then i sleep
            pthread_cond_wait(&cond, &mutex);
        }
        printf("thread2:%d\n", i);
        ++i;
        pthread_mutex_unlock(&mutex);
        //sleep(1);
    }
}
#endif
