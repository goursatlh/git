#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#define RECURSIVE
pthread_mutex_t mutex;
pthread_mutexattr_t attr;
 
void* func(void* arg){
        std::cout<<"thread : "<<pthread_self()<<" going to get lock 1st"<<std::endl;
        pthread_mutex_lock(&mutex);
        std::cout<<"thread : "<<pthread_self()<<" going to get lock 2st"<<std::endl;
        pthread_mutex_lock(&mutex);
        std::cout<<"thread : "<<pthread_self()<<" going to sleep for 4s"<<std::endl;
        sleep(200);
        std::cout<<"thread : "<<pthread_self()<<" going to free lock 1st"<<std::endl;
        pthread_mutex_unlock(&mutex);
        std::cout<<"thread : "<<pthread_self()<<" going to free lock 2st"<<std::endl;
        pthread_mutex_unlock(&mutex);
        return NULL;
}
int main(){
        int ret;
#ifdef RECURSIVE        
        if(( ret = pthread_mutexattr_init(&attr)) != 0){
                fprintf(stderr, "create mutex attribute error. msg:%s", strerror(ret));
                exit(1);
        }
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex, &attr);
#else
        pthread_mutex_init(&mutex, NULL);
#endif
        pthread_t p1, p2;
        if((ret = pthread_create(&p1, NULL, func, NULL)) != 0){
                fprintf(stderr, "create thread error. msg:%s", strerror(ret));
                exit(1);
        }
        sleep(2);
        if((ret = pthread_create(&p2, NULL, func, NULL)) != 0){
                fprintf(stderr, "create thread error. msg:%s", strerror(ret));
                exit(1);
        }
        pthread_join(p1, NULL);
        pthread_join(p2, NULL);
        return 0;
}
