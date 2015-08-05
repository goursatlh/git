

/*
运行这个程序需要预先设置栈内存和文件描述符上限, 否则运行失败
ulimit -n 16384
ulimit -s 4096

文件名:server.c
编译: gcc server.c -Wall -O2 -pthread -o server
程序源码如下(请自行编辑宏定义SERVER_IP为自己的IP):



*/
/*Linux 2.6 x86_64 only*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

#define THREAD_MAX 256
#define LISTEN_MAX 256
#define SERVER_IP "172.16.16.25"

typedef struct 
{
	char ip4[128];
	int port;
	int fd;
} LISTEN_INFO;

static LISTEN_INFO s_listens[LISTEN_MAX];

static unsigned int s_thread_para[THREAD_MAX][8];
static pthread_t s_tid[THREAD_MAX];
pthread_mutex_t s_mutex[THREAD_MAX];


#define LOG(fmt, args...) \
    printf("%s:%d "fmt"\n", __FUNCTION__, __LINE__, ##args);


static int init_thread_pool(void);
static int init_listen4(char *ip4, int port, int max_link);

void * test_server4(unsigned int thread_para[]);

int main(int argc, char *argv[])
{
   int            i, j, rc;
   int            sock_listen;    
   int            sock_cli;    
   int            listen_index;
   int            epfd;
   int             nfds;
   struct epoll_event    ev;
   struct epoll_event    events[LISTEN_MAX];

   socklen_t        addrlen;    
   struct sockaddr_in    addr4; 

   rc = init_thread_pool();
   if (0 != rc) 
       exit(-1);
   
   for(i = 0; i < LISTEN_MAX; i++) {
       sprintf(s_listens[i].ip4, "%s", SERVER_IP);
       s_listens[i].port = 8000 + i;
       rc = init_listen4(s_listens[i].ip4, s_listens[i].port, 64);
       if (0 > rc) {
           fprintf(stderr, "cannot create listen sev on %s:%d\r\n", s_listens[i].ip4, s_listens[i].port);
           exit(-1);
       }
       s_listens[i].fd = rc;
   }
   
   epfd = epoll_create(8192);
   for (i = 0; i < LISTEN_MAX; i++) {
       ev.events = EPOLLIN;
       ev.data.u32 = i;
       if (epoll_ctl(epfd, EPOLL_CTL_ADD, s_listens[i].fd, &ev) < 0) {
           fprintf(stderr, "add fd to epoll failed, (fd =%d)\r\n", rc);
           exit(-1);
       }
   }


   for( ; ; ) {
       LOG("Wait the clients......")
       nfds = epoll_wait(epfd, events, LISTEN_MAX, -1);
       for(i = 0; i < nfds; i++) {
           listen_index = events[i].data.u32;
           sock_listen = s_listens[listen_index].fd;
           addrlen = sizeof(struct sockaddr_in);
           bzero(&addr4, addrlen);
           sock_cli = accept(sock_listen, (struct sockaddr *)&addr4, &addrlen);
           if(0 > sock_cli) {
               fprintf(stderr, "accept err, %d\n", sock_cli);
               continue;
           }
           for(j = 0; j < THREAD_MAX; j++) {
               if (0 == s_thread_para[j][0]) break;
           }
           if (j >= THREAD_MAX) {
               fprintf(stderr, "\r\n");
               shutdown(sock_cli, SHUT_RDWR);
               close(sock_cli);
               continue;
           }
           s_thread_para[j][0] = 1;
           s_thread_para[j][1] = sock_cli;
           s_thread_para[j][2] = listen_index;
           
           pthread_mutex_unlock(s_mutex + j);
       }//end of for(i;;)
   }//end of for(;;)

   exit(0);
}

static int init_thread_pool(void)
{
   int    i, rc;

   LOG();

   for(i = 0; i < THREAD_MAX; i++) {
       s_thread_para[i][0] = 0;
       s_thread_para[i][7] = i;
       pthread_mutex_lock(s_mutex + i);
   }

   for(i = 0; i < THREAD_MAX; i++) {
       rc = pthread_create(s_tid + i, 0, (void *)test_server4, (void *)(s_thread_para[i]));
       if (0 != rc) {
           fprintf(stderr, "create thread %d failed\n", i);
           return(-1);
       }
   }
   
   return(0);
}

static int init_listen4(char *ip4, int port, int max_link)
{
   int            sock_listen4;
   struct sockaddr_in    addr4;
   unsigned int        optval;
   struct linger        optval1;

   bzero(&addr4, sizeof(addr4));
   inet_pton(AF_INET, ip4, &(addr4.sin_addr));
   addr4.sin_family = AF_INET;
   addr4.sin_port = htons(port);
   
   sock_listen4 = socket(AF_INET, SOCK_STREAM, 0);
   if (0 > sock_listen4) return(-1);

   optval = 0x1;
   setsockopt(sock_listen4, SOL_SOCKET, SO_REUSEADDR, &optval, 4);
   
   optval1.l_onoff = 1;
   optval1.l_linger = 60;
   setsockopt(sock_listen4, SOL_SOCKET, SO_LINGER, &optval1, sizeof(struct linger));

   if (0 > bind(sock_listen4, (struct sockaddr *)&addr4, sizeof(addr4))) {
       close(sock_listen4);
       return(-1);
   }

   if (0 > listen(sock_listen4, max_link)) {
       close(sock_listen4);
       return(-1);
   }

   return(sock_listen4);
}


void * test_server4(unsigned int thread_para[])
{
   int        pool_index;    
   int        sock_cli;    
   int        listen_index; 

   char        buff[32768]; 
   char        *p;
   int        i, j, len;
   
   pthread_detach(pthread_self());
   pool_index = thread_para[7];

wait_unlock:

   pthread_mutex_lock(s_mutex + pool_index);
   
   sock_cli = thread_para[1];
   listen_index = thread_para[2];
   
   len = recv(sock_cli, buff, 32768, MSG_NOSIGNAL);

   p = buff;

   p += sprintf(p, "HTTP/1.1 200 OK\r\n");
   p += sprintf(p, "Content-Type: text/html\r\n");
   p += sprintf(p, "Connection: closed\r\n\r\n");
   p += sprintf(p, "<html>\r\n<head>\r\n");
   p += sprintf(p, "<meta content=\"text/html; charset=UTF-8\" http-equiv=\"Content-Type\">\r\n");
   p += sprintf(p, "</head>\r\n");
   p += sprintf(p, "<body style=\"background-color: rgb(229, 229, 229);\">\r\n");
   
   p += sprintf(p, "<center>\r\n");
   p += sprintf(p, "<H3>连接状态</H3>\r\n");
   p += sprintf(p, "<p>服务器地址 %s:%d</p>\r\n", s_listens[listen_index].ip4, s_listens[listen_index].port);
   j = 0;
   for(i = 0; i < THREAD_MAX; i++) {
       if (0 != s_thread_para[i][0]) j++;
   }
   p += sprintf(p, "<H3>线程池状态</H3>\r\n");
   p += sprintf(p, "<p>线程池总数 %d 活动线程总数 %d</p>\r\n", THREAD_MAX, j);
   p += sprintf(p, "</center></body></html>\r\n");
   len = p - buff;

   send(sock_cli, buff, len, MSG_NOSIGNAL);

   shutdown(sock_cli, SHUT_RDWR);
   close(sock_cli);
   
   thread_para[0] = 0;
   goto wait_unlock;

   pthread_exit(NULL);
}


