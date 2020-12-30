#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define CLIENT_TABLE_SOCKNAME  "/home/wt/code/git/IPC/sock/unixsock/socktmp"

int main()
{
    struct sockaddr_un dst_sockaddr;
    struct sockaddr_un src_sockaddr;
    struct sockaddr_un*     dst_sock_un = (struct sockaddr_un *)&dst_sockaddr;
    struct sockaddr_un*     src_sock_un = (struct sockaddr_un *)&src_sockaddr;
    char buff[128] = "hello world";
    int ret = 0;
    int flags = 0;  
    struct timeval tv = {10, 0};

    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    /* bind to random unix socket name, so we can get a response */
    memset(src_sock_un, 0, sizeof(struct sockaddr_un));
    src_sock_un->sun_family = AF_UNIX;
    ret = bind( fd, (void *)src_sock_un, sizeof(struct sockaddr_un) );
    printf("bind ret %d\n", ret);

    ret = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    printf("setsockopt ret %d-%s\n", ret, strerror(errno));
#if 0
    flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags|O_NONBLOCK);
#endif
    /* Create destination name. */
    memset(dst_sock_un, 0, sizeof(struct sockaddr_un));
    dst_sock_un->sun_family = AF_UNIX;
    strcpy(dst_sock_un->sun_path, CLIENT_TABLE_SOCKNAME );

    ret = connect( fd, (struct sockaddr *)dst_sock_un, sizeof(struct sockaddr_un) );
    printf("connect ret %d, err: %s\n", ret, strerror(errno));

    printf("send data to unix socket.\n");
    while (1)
    {
        ret = send(fd, buff, sizeof(buff), 0);
        if (ret > 0)
            printf("send data to unix socket, len %d.\n", ret);
        else
        {
            printf("send ret %d, err: %s\n", ret, strerror(errno));
            //break;
        }
    }
    close(fd);

    return 0;
}
