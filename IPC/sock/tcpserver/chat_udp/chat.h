#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define SEV_IP "172.16.16.25"
#define SEV_PORT 9090
#define MAX_LEN 128

#define LOG(fmt, args...)\
    printf("%s(%d)"fmt"\n", __FUNCTION__, __LINE__, ##args);

extern int flag;

typedef struct threads
{
    	int fd;
	struct sockaddr_in *addr;
}thread_s;

void *send_func(void *arg);

void *recv_func(void *arg);

