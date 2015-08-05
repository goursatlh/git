#include <stdio.h>
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

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd <= 0)
    {
        printf("create socket failed, errcode: %d\n", errno);
        goto EXIT;
    }
   
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("10.88.23.144");
    addr.sin_port = htons(port);

    n = sendto(fd, buff, sizeof(buff), 0, (struct sockaddr *)(&addr), sizeof(addr));
    if (n > 0)
    {
        printf("sendto return %d\n", n);
    }

EXIT:
    if (fd > 0)
    {
        close(fd);
    }
    return 0;
}
