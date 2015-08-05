#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // this must add, otherwise the inet_ntoa can't work in 64 system

int main(int argc, char **argv)
{
    int fd = 0;
    int ret = 0;
    int port = atoi(argv[1]);
    char buff[128] = {0};
    struct sockaddr_in server;
    struct sockaddr_in client;
    int clientlen = sizeof(client);

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd <= 0)
    {
        printf("create socket failed, errcode: %d\n", errno);
        goto EXIT;
    }
    printf("create socket successfully, socket fd: %d\n", fd);
   
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    ret = bind(fd, (struct sockaddr *)(&server), sizeof(server));
    if (ret < 0)
    {
        printf("bind socket failed, errcode: %d\n", errno);
        goto EXIT;
    }
    printf("bind port: %d\n", port);

    memset(&client, 0, sizeof(client));
    ret = recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)(&client), &clientlen);
    if (ret < 0)
    {
        printf("sendto failed, errcode %d\n", errno);
    }
    else
    {
        printf("recv %d bytes data from (%s/%d): %s\n", ret, inet_ntoa(client.sin_addr), ntohs(client.sin_port), buff);
    }

EXIT:
    if (fd > 0)
    {
        close(fd);
    }
    return 0;
}
