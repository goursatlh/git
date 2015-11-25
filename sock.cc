#include <iostream>
#include <sys/socket.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    int type = atoi(argv[1]);
    int port = atoi(argv[2]);
    unsigned long sendnum = atoi(argv[3]);
    int ret = 0;
    struct sockaddr_in ser_addr;
    struct sockaddr_in cli_addr;
    int addrlen = sizeof(struct sockaddr_in);
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int fd2 = 0;
    cout<<"fd "<<fd<<" created"<<endl;
    
    if (type == 1)
    {
        cout<<"server start: "<<endl;
        ser_addr.sin_port = port;
        ser_addr.sin_family = AF_INET;
        
        ret = bind(fd, &ser_addr, addrlen);
        cout<<"bind "<<endl;
   
        ret = listen(fd, 5);
        fd2 = accpet(fd, &ser_addr, );
        ret = recv(fd2, );
    }
    else if (type == 0)
    {
        cout<<"client start: "<<endl;
    }
    return 0;
}
