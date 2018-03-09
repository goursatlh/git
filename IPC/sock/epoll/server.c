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
#include <time.h>
#include <sys/epoll.h>
#include <sys/resource.h>

#define ___PORT 3838
#define ___epoll_parall_events (20)
#define ___epoll_min (10)
static unsigned long ___epoll_max = 1024;
static int sockfd_server;
static int *epollfds;
static int epfd;
static int max_fd = 0;
static struct epoll_event eevents[___epoll_parall_events];
 inline void fd_set2(int fd);
inline int fd_get2(int ifd);
inline void fd_clr2(int ifd);
inline void fd_zero2(void);
inline int fd_isset2(int ifd);
inline int fd_isin(int ifd);
void process_data(int sockfd, char *buf);
 int main(int argc, char *argv[]) 
{
	int newsockfd;
	struct sockaddr_in addr;
	int addr_len;

	time_t local_time;
	int readbytes;
	int ret_nums, ifd;
	struct rlimit lim;
	unsigned long user_nums;
	 char buffer[256];
	char msg[] = "Welcome 2gliethttp server!";

	 addr_len = sizeof(struct sockaddr_in);
	if ((sockfd_server = socket(PF_INET, SOCK_STREAM, 0)) < 0)	//申请套接字
	{
		perror("socket");
		return 0;
	}
	 memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(___PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//处理来自PC上的任何一块网卡数据
	if (bind(sockfd_server, (struct sockaddr *)&addr, sizeof(addr)) < 0)	//将套接字和配置参数绑定在一起
	{
		perror("connect");
		return 0;
	}
	 if (listen(sockfd_server, 5) < 0)	//同一时刻最多允许5个连接
	{
		perror("listen");
		return 0;
	}
	 if (argc > 1)
		user_nums = strtol(argv[1], 0, 0);
	
	else
		user_nums = 0;
	lim.rlim_cur = user_nums + ___epoll_min;
	lim.rlim_max = user_nums + ___epoll_min;
	if (setrlimit(RLIMIT_NOFILE, &lim))
		
	{
		perror("setrlimit");
		exit(0);
	}
	___epoll_max = user_nums + ___epoll_min - 1;
	 printf("epoll_max=%d\n", ___epoll_max);
	fflush(stdout);
	 epfd = epoll_create(___epoll_max);
	epollfds = malloc(sizeof(int) * ___epoll_max);
	 fd_zero2();
	fd_set2(sockfd_server);
	 max_fd = epfd;
	 for (;;)
		
	{
		printf("\nepolling...\n");
		 ret_nums =
		    epoll_wait(epfd, eevents,
			       sizeof(eevents) / sizeof(eevents[0]), -1);
		if (ret_nums <= 0)
			
		{
			perror("epoll error: ");
			exit(-1);
		}
		 printf("ret_nums=%d\n", ret_nums);
		fflush(stdout);
		 for (ifd = 0; ifd < ret_nums; ifd++)
			
		{
			if (fd_isset2(ifd))
				
			{
				if (sockfd_server == fd_get2(ifd))
					
				{
					
					    //监听端口有连接触发请求了,那么处理之
					    if ((newsockfd =
						 accept(sockfd_server,
							(struct sockaddr *)
							&addr,
							(socklen_t *) &
							addr_len)) < 0)
						perror("accept");
					
					else
						
					{
						if (++max_fd >= ___epoll_max)
							
						{
							max_fd = ___epoll_max;
							fd_clr2(ifd);
						}
						 fprintf(stderr,
							   "=====%d=====\n",
							   newsockfd);
						fd_set2(newsockfd);
						time(&local_time);
						sprintf(buffer, "<%d> : %s %s",
							 newsockfd, msg,
							 ctime(&local_time));
						write(newsockfd, buffer,
						       strlen(buffer));
						printf
						    ("conect: <%d> from %s %s\n",
						     newsockfd,
						     inet_ntoa(addr.sin_addr),
						     ctime(&local_time));
					}
				}
				
				else
					
				{
					readbytes =
					    read(fd_get2(ifd), buffer,
						 sizeof(buffer));
					if (readbytes <= 0)
						
					{
						time(&local_time);
						printf("closed: <%d> %s\n",
							fd_get2(ifd),
							ctime(&local_time));
						fd_clr2(ifd);
						close(fd_get2(ifd));
						 if (max_fd-- == ___epoll_max)
							
						{
							fd_set2(sockfd_server);
						}
					}
					
					else
						
					{
						buffer[readbytes] = 0;
						printf("recv: %s\n", buffer);
						process_data(fd_get2(ifd),
							      buffer);
					}
				}
			}
		}
	}
}

 inline void fd_set2(int fd) 
{
	struct epoll_event eevent;

	eevent.data.fd = fd;
	eevent.events = EPOLLIN;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, eevent.data.fd, &eevent))
		
	{
		perror("epoll_ctl(ADD)");
		return;
	}
	int ifd;

	ifd = fd;		//+ 100;
	epollfds[ifd] = fd;
} inline int fd_get2(int ifd) 
{
	return eevents[ifd].data.fd;
}

void fd_remove(int fd) 
{
	int i;

	for (i = 0; i < ___epoll_max; i++)
		
	{
		if (epollfds[i] == fd)
			
		{
			epollfds[i] = 0;
			break;
		}
	}
}

inline void fd_clr2(int ifd) 
{
	struct epoll_event eevent;

	eevent.data.fd = eevents[ifd].data.fd;
	fd_remove(eevent.data.fd);
	if (epoll_ctl(epfd, EPOLL_CTL_DEL, eevent.data.fd, &eevent))
		
	{
		perror("epoll_ctl(DEL)");
		return;
	}
}

inline void fd_zero2(void) 
{
	
#if 0
	int i;

	for (i = 0; i < ___poll_max; i++)
		
	{
		pfds[i].fd = 0;
		pfds[i].events = POLLNVAL;
		pfds[i].revents = 0;
	}
	
#endif	/*  */
	    //memset(pfds, 0, sizeof(pfds));
}

inline int fd_isset2(int ifd) 
{
	return 1;
}

inline int fd_isin(int ifd) 
{
	return epollfds[ifd];
}

 void process_data(int sockfd, char *buf) 
{
	char *pdata;
	int idest_sock, dest_sock;
	int tmp_len;
	char buffer[1024 * 10];
	int ifd;

	 if (strncmp("ls", buf, 2) == 0)
		
	{
		char *p;

		p = buffer;
		for (ifd = 0; ifd < ___epoll_max; ifd++)
			
		{
			if (fd_isin(ifd))
				
			{
				if (fd_isin(ifd) == sockfd)
					p += sprintf(p, "#%d*", ifd);
				p += sprintf(p, "%d\n", fd_isin(ifd));
			}
		}
		write(sockfd, buffer, strlen(buffer));
		return;
	}
	 
#if 0
	    pdata = strtok(buf, ":");
	if (pdata == NULL)
		
	{
	 }
	
	else
		
#endif	/*  */
	{
		pdata = buf;
		dest_sock = strtol(pdata, 0, 0);	//atoi(pdata);strtol
		idest_sock = ___epoll_max;
		for (ifd = 0; ifd < ___epoll_max; ifd++)
			
		{
			if (fd_isin(ifd) == dest_sock)
				
			{
				idest_sock = ifd;
				break;
			}
		}
		 if (idest_sock < ___epoll_max)
			
		{
			if (dest_sock < 10)
				tmp_len = 1;
			
			else if (dest_sock < 100)
				tmp_len = 2;
			
			else if (dest_sock < 1000)
				tmp_len = 3;
			
			else
				tmp_len = 4;
			
			    //pdata = strtok(buf, ":");
			    //pdata += strlen(pdata) + 1;
			    pdata += tmp_len;
			 if (dest_sock == sockfd_server)
				
			{
				printf("<%d> %s", sockfd, pdata);
				fflush(stdout);
			}
			
			else
				
			{
				sprintf(buffer, "<%d> : %s", sockfd, pdata);
				pdata = buffer;
				write(dest_sock, pdata, strlen(pdata));
			}
		}
		
		else
			
		{
			sprintf(buffer, "client : %d not exist\n", dest_sock);
			write(sockfd, buffer, strlen(buffer));
		}
	}
}


