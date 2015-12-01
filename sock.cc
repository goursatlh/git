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

using std::cout;
using std::endl;
using std::thread;

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

        if (mode == 1)
        {
            fd_set rfds;
            int fdset[1024] = {0};
            int num = 0;
            cout<<"server is in non-locking mode"<<endl;
            ret = fcntl(fd, F_SETFL, O_NONBLOCK);
            if (ret == -1)
            {
                cout<<"fcntl failed, err: "<<strerror(errno)<<endl;
                goto Exit;
            }
            FD_ZERO(&rfds);
            cout<<"begin to accept clients: "<<endl;
            while (1)
            {
                sleep(2);
                fd2 = accept(fd, (struct sockaddr *)(&cli_addr), &addrlen);
                if (fd2 > 0)
                {
                    cout<<"accept from "<<inet_ntoa(cli_addr.sin_addr)<<endl;
                    FD_SET(fd2, &rfds);
                    fdset[num] = fd2;
                    num++;
                }
                if (num > 0)
                {
                    for (int i = 0; i < num; i++)
                    {
                        if (fdset[i] > 0)
                        {
                            ret = select(fdset[i]+1, &rfds, NULL, NULL, NULL);
                            if (ret > 0)
                            {
                                if (FD_ISSET(fdset[i], &rfds))
                                {
                                    size_t recvsum = 0, rb = 0;
                                    char buff[8192] = {0};
                                    while ((rb = recv(fdset[i], buff, sizeof(buff), 0)) > 0)
                                    {
                                        recvsum += rb;
                                    }
                                    cout<<"recv "<<recvsum<<" bytes from client "<<endl;
                                    close(fdset[i]);
                                    fdset[i] = 0;
                                    break;
                                }
                            }
                            else if (ret == 0)
                            {
                                cout<<"select timeout"<<endl;    
                            }
                            else
                            {
                                cout<<"select return -1, err: "<<strerror(errno)<<endl;    
                            }
                        }
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
        size_t nr = 0, sb = 0;
        size_t sendsum = 0;
        struct linger linger;
        cout<<"client start: "<<endl;
        cout<<"connect to "<<argv[2]<<":"<<argv[3]<<endl;
#if 0
        linger.l_onoff = 1;
        linger.l_linger = 0;
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
       
        pfile = fopen(argv[4], "rb");
        if (pfile == NULL)
            goto Exit; 
        while ((nr = fread(buff, 1, sizeof(buff), pfile)) > 0)
        {
            sb = send(fd, buff, nr, 0);
            sendsum += sb;
        }
        fclose(pfile);
        cout<<"send "<<sendsum<<" bytes from file "<<argv[4]<<endl;
#if 0
        shutdown(fd, SHUT_WR);
        ret = recv(fd, buff, 4096, 0);
        if (ret == 0)
            goto Exit;
#endif
    }
Exit:
    if (fd)
        close(fd);
    if (fd2)
        close(fd2);
    return 0;
}
