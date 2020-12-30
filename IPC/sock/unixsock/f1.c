#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>

#define CLIENT_TABLE_SOCKNAME  "/home/wt/code/git/IPC/sock/unixsock/socktmp"

int main()
{
    struct sockaddr_un   src_sockaddr;
    struct sockaddr_un*  src_sock_un = (struct sockaddr_un *)&src_sockaddr;
    char buff[128] = {0};
    int ret = 0;
    
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    /* bind to random unix socket name, so we can get a response */
    memset(src_sock_un, 0, sizeof(struct sockaddr_un));
    src_sock_un->sun_family = AF_UNIX;
    strcpy(src_sock_un->sun_path, CLIENT_TABLE_SOCKNAME );
    ret = unlink(CLIENT_TABLE_SOCKNAME);
    printf("unlink %s, ret %d-%s\n", CLIENT_TABLE_SOCKNAME, ret, strerror(errno));
    ret = bind( fd, (const struct sockaddr *)src_sock_un, sizeof(struct sockaddr_un) );
    printf("sock %d bind to %s, ret %d - %s\n", fd, CLIENT_TABLE_SOCKNAME, ret, strerror(errno) );
    
    while (1)
    {
#if 0
        ret = recvfrom(fd, buff, sizeof(buff), 0, NULL, NULL);
        if (ret > 0)
        {
            printf("recv data %d : %s\n", ret, buff);
        }
#endif
        sleep(3);
    }
    return 0;
}
