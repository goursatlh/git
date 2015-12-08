#include <iostream>
#include <sys/socket.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <thread>
#include <sys/time.h>
#include <sys/epoll.h>

using std::cout;
using std::endl;
using std::thread;
#define exit_proc(func, ret) \
    if (ret == -1)\
    {\
        cout<<#func" "<<__LINE__<<" failed, err: "<<strerror(errno)<<endl;\
        goto Exit;\
    }\

void recv_thread(int fd)
{
    size_t recvsum = 0, rb = 0;
    char buff[8192] = {0};
    cout<<"recv thread start, pid: "<<pthread_self()<<endl;
    while ((rb = recv(fd, buff, sizeof(buff), 0)) > 0)
    {
        recvsum += rb;
    }
    cout<<"recv "<<recvsum<<" bytes from client "<<endl;
    close(fd);
}

void set_nonblock(int fd)
{
    int ret = 0;
    ret = fcntl(fd, F_SETFL, O_NONBLOCK);
    exit_proc(set_nonblock, ret);
Exit:
    return;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout<<"usage: ./sock sock_type addr port [ser_type(server)]/[file_name(client)]"<<endl;
        return -1;
    }
    int type = atoi(argv[1]);
    int port = atoi(argv[3]);
    struct sockaddr_in ser_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    char buff[8192] = {0};
    int ret = 0;
    int fd2 = 0;
    int i = 0;

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    cout<<"fd "<<fd<<" created"<<endl;
    
    if (type == 1)
    {
        int mode = atoi(argv[4]);
        struct sockaddr_in cli_addr;
        ser_addr.sin_port = htons(port);
        ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
        ser_addr.sin_family = AF_INET;
        cout<<"server start: "<<endl;
        ret = bind(fd, (struct sockaddr *)(&ser_addr), addrlen);
        if (ret != 0)
        {
            cout<<"bind failed, err: "<<strerror(errno)<<endl;
            goto Exit;
        }
  
        cout<<"listen on "<<port<<endl;
        ret = listen(fd, 5);

        if (mode == 1) //non-blocking select
        {
            fd_set rfds, master;
            int maxfd = fd;
            int nready = 0, ret = 0;
            cout<<"server is in non-locking mode"<<endl;
            
            set_nonblock(fd);
            
            FD_ZERO(&rfds);
            FD_ZERO(&master);
            FD_SET(fd, &master);
            
            while (1)
            {
                memcpy(&rfds, &master, sizeof(master));
                nready = select(maxfd+1, &rfds, NULL, NULL, NULL);
                if (nready > 0)
                {
                    for (int i = 0; i <= maxfd, nready > 0; i++)
                    {
                        if (FD_ISSET(i, &rfds))
                        {
                            nready--;
                            if (i == fd)
                            {
                                fd2 = accept(fd, (struct sockaddr *)(&cli_addr), &addrlen);
                                if (fd2 > 0)
                                {
                                    cout<<"accept from "<<inet_ntoa(cli_addr.sin_addr)<<endl;
                                    FD_SET(fd2, &master);
                                    set_nonblock(fd2);
                                    if (fd2 > maxfd)
                                        maxfd = fd2;
                                }
                            }
                            else
                            {
                                size_t recvsum = 0, rb = 0;
                                char buff[8192] = {0};
                                while ((rb = recv(i, buff, sizeof(buff), 0)) > 0)
                                {
                                    recvsum += rb;
                                }
                                cout<<"recv "<<recvsum<<" bytes from client "<<endl;
                                close(i);
                                FD_CLR(i, &master);
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (mode == 2) //non-blocking epoll
        {
            int epfd = 0, nfds = 0;
            struct epoll_event ev = {0}, events[20];
            memset(events, 0, sizeof(events));

            cout<<"server is in non-blocking epoll mode"<<endl;
            set_nonblock(fd);
            epfd = epoll_create(256);
            exit_proc(epoll_create, epfd);
            
            ev.data.fd = fd;
            ev.events = EPOLLIN | EPOLLET;
            ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
            exit_proc(epoll_ctl, epfd);
            
            while (1)
            {
                nfds = epoll_wait(epfd, events, 20, 500);
                for (i = 0; i < nfds; i++)
                {
                    if (events[i].data.fd == fd)
                    {
                        fd2 = accept(fd, (struct sockaddr *)(&cli_addr), &addrlen);
                        if (fd2 > 0)
                        {
                            cout<<"accept from "<<inet_ntoa(cli_addr.sin_addr)<<" port: "<<cli_addr.sin_port<<" new fd: "<<fd2<<endl;
                            set_nonblock(fd2);
                            
                            ev.data.fd = fd2;
                            ev.events = EPOLLIN | EPOLLET;
                            ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);
                            exit_proc(epoll_ctl, ret);
                        }
                    }
                    else if (events[i].events == EPOLLIN)
                    {
                        size_t recvsum = 0, rb = 0;
                        int fd3 = events[i].data.fd;
                        cout<<"recv the read event for fd "<<fd3<<endl;
                        if (fd3 < 0)
                            continue;
                        while ((rb = recv(fd3, buff, sizeof(buff), 0)) > 0)
                        {
                            recvsum += rb;
                        }
                        cout<<"recv "<<recvsum<<" bytes from client "<<endl;
                        events[i].data.fd = -1;
                        ev.data.fd = fd3;
                        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd3, &ev);
                        exit_proc(epoll_ctl, ret);
                        //close(fd3);
                    }
                }
            }
        }
        else
        {
            cout<<"server is in blocking mode: one thread per connection"<<endl;
            while ((fd2 = accept(fd, (struct sockaddr *)(&cli_addr), &addrlen)) > 0)
            {
                cout<<"accept from "<<inet_ntoa(cli_addr.sin_addr)<<endl;
                thread t(recv_thread, fd2);
                t.detach();
                //t.join();
                //cout<<"pid "<<t.get_id()<<endl;
            }
        }
    }
    else if (type == 2)
    {
        FILE *pfile = NULL;
        struct timeval tvstart, tvend;
        long timespend = 0;
        size_t nr = 0, sb = 0;
        size_t sendsum = 0;
        struct linger linger;
        cout<<"client start: "<<endl;
        cout<<"connect to "<<argv[2]<<":"<<argv[3]<<endl;
#if 1
        linger.l_onoff = 1;
        linger.l_linger = 30;
        ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(struct linger));
        cout<<"setsockopt ret "<<ret<<endl;
#endif
        ser_addr.sin_port = htons(port);
        ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
        ser_addr.sin_family = AF_INET;
        ret = connect(fd, (struct sockaddr *)(&ser_addr), addrlen);
        if (ret == -1)
        {
            cout<<"connect failed, err: "<<strerror(errno)<<endl;
            goto Exit;
        }
       
        gettimeofday( &tvstart, NULL);
        pfile = fopen(argv[4], "rb");
        if (pfile == NULL)
            goto Exit; 
        while ((nr = fread(buff, 1, sizeof(buff), pfile)) > 0)
        {
            sb = send(fd, buff, nr, 0);
            sendsum += sb;
        }
        fclose(pfile);
        gettimeofday( &tvend, NULL);
        timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
        cout<<"\nsend "<<sendsum<<" bytes from file "<<argv[4]<<" time spent: "<<timespend<<"us"<<endl;
#if 0
        //shutdown(fd, SHUT_WR);
        ret = recv(fd, buff, 4096, 0);
        if (ret == 0)
        {
            cout<<"recv 0 note that peer close the sock"<<endl;
            goto Exit;
        }
#endif
    }
Exit:
    if (fd)
        close(fd);
    if (fd2)
        close(fd2);
    return 0;
}
