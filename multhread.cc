#if 0 // mutex for bit filed:  we should lock whole of the int, not bit field part
#include <iostream>
#include <thread>
#include <unistd.h>

using std::cout;
using std::endl;
using std::thread;

#define MAX_LOOP 100000000
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mymutex1 = PTHREAD_MUTEX_INITIALIZER;
struct foo
{
    long flag:1;
    long counter:31;
}my_foo;

void func1()
{
    int i = 0;
    cout<<"thread1 begin"<<endl;
    while (i++ < MAX_LOOP)
    {
        pthread_mutex_lock(&mymutex);
        my_foo.flag = !my_foo.flag;
        pthread_mutex_unlock(&mymutex);
    }
}

void func2()
{
    int i = 0;
    cout<<"thread2 begin"<<endl;
    while (i++ < MAX_LOOP)
    {
        pthread_mutex_lock(&mymutex1); // we should use mymutex, not mymutex1
        my_foo.counter++;
        pthread_mutex_unlock(&mymutex1);
    }
}
int main()
{
       cout<<"size "<<sizeof(my_foo)<<endl;
       thread t1(func1); 
       thread t2(func2);
       cout<<"begin to join thread1"<<endl;
       t1.join(); //join() is a blocking opt until thread return
       cout<<"begin to join thread2"<<endl;
       t2.join();
       cout<<"result: "<<my_foo.flag<<" "<<my_foo.counter<<endl;
       return 0;
}
#endif
#if 0 // mutex
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int myglobal;
#define LOOP_MAX 10000
struct foo
{
    int flag:1;
    int counter:15;
}my_foo;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_flag = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_counter = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg)
{
    int i,j;
    for ( i=0; i<LOOP_MAX; i++ ) {
        pthread_mutex_lock(&mymutex);
        j=myglobal;
        j=j+1;
        //printf(".");
        //fflush(stdout);
        //sleep(1);
        myglobal=j;
        pthread_mutex_unlock(&mymutex);
        my_foo.flag = !my_foo.flag;
    }
    return NULL;
}

int main(void)
{
    pthread_t mythread;
    int i;
    my_foo.flag = 0;
    my_foo.counter = 0;
    printf("size %ld\n", sizeof(my_foo));

    if ( pthread_create( &mythread, NULL, thread_function, NULL) ) {
        printf("error creating thread.");
        abort();
    }

    for ( i=0; i<LOOP_MAX; i++) {
        pthread_mutex_lock(&mymutex);
        myglobal=myglobal+1;
        pthread_mutex_unlock(&mymutex);
        my_foo.counter++;
        //printf("o");
        //fflush(stdout);
        //sleep(1);
    }

    if ( pthread_join ( mythread, NULL ) ) {
        printf("error joining thread.");
        abort();
    }

    printf("\nmyglobal equals %d\n",myglobal);
    printf("\nmyglobal flag %d, counter %d\n",my_foo.flag, my_foo.counter);
    exit(0);
}
#endif

#if 0 // condition
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
