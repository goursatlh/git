#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

#define STDIN 0
#define MAXDATASIZE 10240
#define ___epoll_max 2
#define ___epoll_parall_events 4

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    char send_str[MAXDATASIZE];
    int recvbytes;
    int sockfd,k;
    struct hostent *hp;
    struct sockaddr_in addr;
    char *ip;
    int port;
    int ret_nums,i;
    int epfd;
    struct epoll_event eevents[___epoll_parall_events];

    if(argc < 3)
    {
        printf("client ip port\n");
        return -1;
    }
    ip = argv[1];
    port = atoi(argv[2]);//strtol(argv[2], 0, 0);
    hp = gethostbyname(ip);
    if (hp == NULL)
    {
        perror("hostent error" );
        return -2;
    }

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket error" );
        return( -1 );
    }

    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family=PF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
    //memcpy(&addr.sin_addr,hp->h_addr_list[0],hp->h_length);
    //addr.sin_addr.s_addr= htonl(INADDR_ANY);
    printf("Connecting %s:%d ....\n",ip,port);
    fflush(stdout);
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
    epfd = epoll_create(___epoll_max);
    
    eevents[0].events = EPOLLIN;
    eevents[0].data.fd = STDIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, eevents[0].data.fd, &eevents[0]);

    eevents[0].events = EPOLLIN;
    eevents[0].data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, eevents[0].data.fd, &eevents[0]);

    for(;;)
    {
        ret_nums = epoll_wait(epfd, eevents, sizeof(eevents)/sizeof(eevents[0]), -1);
        if(ret_nums <= 0)
        {
            perror("epoll error: ");
            exit(-1);
        }
        
        for(i = 0;i < ret_nums;i++)//如果之允许同时返回1个event,那么这个循环就没用了,因为ret_nums始终为1
        {
            if(eevents[i].data.fd == STDIN)
            {
                fgets(send_str, MAXDATASIZE, stdin);
                send_str[strlen(send_str) - 1] = 0;
                if(strncmp("quit", send_str, 4) == 0)
                {
                    close(sockfd);
                    exit(0);
                }
                printf("send: %s\n", send_str);
                send(sockfd, send_str, strlen(send_str), 0);
            }

            if(eevents[i].data.fd == sockfd)
            {
                recvbytes = recv(sockfd, buf, MAXDATASIZE, 0);
                if(recvbytes <= 0)
                {
                    close(sockfd);
                    exit(0);
                }
                buf[recvbytes] = 0;
                printf("-----------\n%s\n", buf);
                fflush(stdout);
            }
        }
    }
}
