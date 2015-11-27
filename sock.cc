#include <iostream>
#include <sys/socket.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout<<"usage: ./sock type port addr"<<endl;
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
        size_t recvsum = 0;
        struct sockaddr_in cli_addr;
        ser_addr.sin_port = htons(port);
        ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
        ser_addr.sin_family = AF_INET;
        cout<<"server start: "<<endl;
        ret = bind(fd, (struct sockaddr *)(&ser_addr), addrlen);
        cout<<"bind "<<ret<<endl;
  
        cout<<"listen on "<<port<<endl;
        ret = listen(fd, 5);
        fd2 = accept(fd, (struct sockaddr *)(&cli_addr), &addrlen);
        if (fd2 > 0)
        {
            while ((ret = recv(fd2, buff, sizeof(buff), 0)) > 0)
            {
                recvsum += ret;
            }
            cout<<"recv "<<recvsum<<" bytes from client "<<inet_ntoa(cli_addr.sin_addr)<<endl;
        }
    }
    else if (type == 2)
    {
        FILE *pfile = NULL;
        size_t nr = 0;
        size_t sendsum = 0;
        struct linger linger;
        cout<<"client start: "<<endl;
        cout<<"connect to "<<argv[2]<<":"<<argv[3]<<endl;

        linger.l_onoff = 1;
        linger.l_linger = 0;
        //ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(struct linger));
        cout<<"setsockopt ret "<<ret<<endl;
        ser_addr.sin_port = htons(port);
        ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
        ser_addr.sin_family = AF_INET;
        ret = connect(fd, (struct sockaddr *)(&ser_addr), addrlen);
        if (ret == -1)
        {
            cout<<strerror(errno)<<endl;
            goto Exit;
        }
       
        pfile = fopen(argv[4], "rb");
        if (pfile == NULL)
            goto Exit; 
        while ((nr = fread(buff, 1, sizeof(buff), pfile)) > 0)
        {
            ret = send(fd, buff, nr, 0);
            sendsum += ret;
        }
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
