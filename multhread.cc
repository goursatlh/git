#if 0
#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <vector>         // std::vector

std::atomic<bool> ready(false);  // 由 false 初始化一个 std::atomic<bool> 类型的原子变量
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void do_count1m(int id)
{
    while (!ready) { std::this_thread::yield(); } // 等待 ready 变为 true.

    for (volatile int i=0; i<1000000; ++i) {} // 计数

    if (!winner.test_and_set()) {
        std::cout << "thread #" << id << " won!\n";
    }
}

int main ()
{
    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i=1; i<=10; ++i) 
        threads.push_back(std::thread(do_count1m,i));
    ready = true;

    for (auto& th : threads) 
        th.join();
    return 0;
}

#endif

#if 0 //atomic_flag as a spinning lock
#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <sstream>        // std::stringstream

std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;
std::stringstream stream;

void append_number(int x) 
{
    while (lock_stream.test_and_set()) { }
    stream << "thread #" << x << '\n';
    lock_stream.clear();
}

int main ()
{
    std::vector<std::thread> threads;
    for (int i=1; i<=10; ++i) 
        threads.push_back(std::thread(append_number,i));
    for (auto& th : threads) 
        th.join();

    std::cout << stream.str();
    return 0;
}
#endif

#if 0 // pthread_spin_lock vs pthread_mutex_lock
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <list>
#include <pthread.h>

#define LOOPS 20000000
using namespace std;
list<int> the_list;

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif

pid_t gettid() { return syscall( __NR_gettid ); }
void *consumer(void *ptr)
{
    int i;
    printf("Consumer TID %lu\n", (unsigned long)gettid());
    while (1)
    {
#ifdef USE_SPINLOCK
        pthread_spin_lock(&spinlock);
#else
        pthread_mutex_lock(&mutex);
#endif
        if (the_list.empty())
        {
#ifdef USE_SPINLOCK
            pthread_spin_unlock(&spinlock);
#else
            pthread_mutex_unlock(&mutex);
#endif
            break;
        }

        i = the_list.front();
        the_list.pop_front();

#ifdef USE_SPINLOCK
        pthread_spin_unlock(&spinlock);
#else
        pthread_mutex_unlock(&mutex);
#endif
    }
    return NULL;
}


int main()
{
    int i;
    pthread_t thr1, thr2;
    struct timeval tv1, tv2;

#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, 0);
#else
    pthread_mutex_init(&mutex, NULL);
#endif
    
    for (i = 0; i < LOOPS; i++)
        the_list.push_back(i);
    gettimeofday(&tv1, NULL);
    pthread_create(&thr1, NULL, consumer, NULL);
    pthread_create(&thr2, NULL, consumer, NULL);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);
    gettimeofday(&tv2, NULL);

    if (tv1.tv_usec > tv2.tv_usec)
    {
        tv2.tv_sec--;
        tv2.tv_usec += 1000000;
    }
    printf("Result - %ld.%ld\n", tv2.tv_sec - tv1.tv_sec,
            tv2.tv_usec - tv1.tv_usec);
#ifdef USE_SPINLOCK
    pthread_spin_destroy(&spinlock);
#else
    pthread_mutex_destroy(&mutex);
#endif
    return 0;
}

#endif

#if 0 // atomic APIs in gcc 
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>

#define INC_TO 1000000 // one million...

int global_int = 0;
pid_t gettid( void )
{
    return syscall( __NR_gettid );
}

void *thread_routine( void *arg )
{
    int i;
    int proc_num = (int)(long)arg;
    cpu_set_t set;
    CPU_ZERO( &set );

    CPU_SET( proc_num, &set );
    if (sched_setaffinity( gettid(), sizeof( cpu_set_t ), &set ))
    {
        perror( "sched_setaffinity" );
        return NULL;
    }
    for (i = 0; i < INC_TO; i++)
    {

        //global_int++;
        __sync_fetch_and_add( &global_int, 2 );
    }
    return NULL;
}

int main()
{
    int procs = 0;
    int i;
    int ret = 0;
    pthread_t *thrs;

    procs = (int)sysconf( _SC_NPROCESSORS_ONLN );
    if (procs < 0)
    {
        perror( "sysconf" );
        return -1;
    }

    thrs = (pthread_t *)malloc( sizeof( pthread_t ) * procs );
    if (thrs == NULL)
    {
        perror( "malloc" );
        return -1;
    }
    printf( "Starting %d threads...\n", procs );

    for (i = 0; i < procs; i++)
    {
        if (pthread_create( &thrs[i], NULL, thread_routine,
                    (void *)(long)i ))
        {
            perror( "pthread_create" );
            procs = i;
            break;
        }
    }

    for (i = 0; i < procs; i++)
        pthread_join( thrs[i], NULL );
    free( thrs );
    printf( "After doing all the math, global_int value is: %d\n",
            global_int );
    printf( "Expected value is: %d\n", INC_TO * procs );
    ret = __sync_lock_test_and_set(&global_int, 0);
    printf("pre %d, aft %d\n", ret, global_int);
    ret = __sync_lock_test_and_set(&global_int, 0);
    printf("pre %d, aft %d\n", ret, global_int);
    return 0;
}

#endif

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

#if 0// no lock
#include <stdio.h>
#include <pthread.h>

#define MAX 10
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
    printf("i %d\n", i);
    return 0;
}


void *thread1(void *junk)
{       
    while (i < MAX)
    {       
        printf(" thread1 before:%d ", i);
        if (i % 3 != 0)
        {       
            printf(" thead1:%d ", i);
            ++i;
        }
    }
}

void *thread2(void *junk)
{
    while (i < MAX)
    {
        printf(" thread2 before:%d ", i);
        if (i % 3 == 0)
        {
            printf(" thread2:%d ", i);
            ++i;
        }
    }
}
#endif

#if 1 // how to put the reference to a thread parameter
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
using std::cout;
using std::endl;
using std::thread;
using std::vector;

template <typename T>
void func(vector<T> &vec, int a, int b, int c)
{
    cout<<a<<" "<<b<<" "<<c<<endl;
    sleep(10);
}


int main()
{
    vector<int> vec;
    thread th1(func<int>, std::ref(vec), 1, 2, 3);
    th1.join();
    cout<<"end"<<endl;
    return 0;
}
#endif
