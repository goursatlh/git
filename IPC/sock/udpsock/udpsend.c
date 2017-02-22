
#if 1
/* udp data transport: does one udp socket use the same src port to send different dip/dport */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int fd = 0;
    int n = 0;
    int port = atoi(argv[1]);
    char buff[] = "hello world.";
    struct sockaddr_in addr;
    struct sockaddr_in addr1;

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd <= 0)
    {
        printf("create socket failed, errcode: %d\n", errno);
        goto EXIT;
    }
   
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("239.255.255.250");
    addr.sin_port = htons(port);

    memset(&addr1, 0, sizeof(addr));
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = inet_addr("10.88.21.132");
    addr1.sin_port = htons(port);
#if 0 
    n = sendto(fd, buff, sizeof(buff), 0, (struct sockaddr *)(&addr), sizeof(addr));
    if (n > 0)
    {
        printf("sendto %d bytes to (239.255.255.250/%d)\n", n, port);
    }
    else
    {
        printf("sendto return %s\n", strerror(errno));
    }
#endif
#if 1
    n = sendto(fd, buff, sizeof(buff), 0, (struct sockaddr *)(&addr1), sizeof(addr1));
    if (n > 0)
    {
        printf("sendto %d bytes to (10.88.21.132/%d)\n", n, port);
    }
    else
    {
        printf("sendto return %s\n", strerror(errno));
    }
#endif
EXIT:
    if (fd > 0)
    {
        close(fd);
    }
    
    return 0;
}
#endif



#if 0
int main(int argc, char* argv[])
{
    struct in_addr addr1,addr2;    
    unsigned long l1,l2;
    l1= inet_addr("192.168.0.74"); 
    l2 = inet_addr("211.100.21.179"); 
    memcpy(&addr1, &l1, 4);
    memcpy(&addr2, &l2, 4);
    printf("%s : %s\n", inet_ntoa(addr1), inet_ntoa(addr2));

    printf("%s\n", inet_ntoa(addr1));
    printf("%s\n", inet_ntoa(addr2));
    return 0;
}
#endif
