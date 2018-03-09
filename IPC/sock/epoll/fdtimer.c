#include <stdio.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

void expirefunc()
{
	printf("timeout expire\n");
}


#define MAX_EVENT 10
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: ./xx timeout\n");
		return 0;
	}
	int timeout = atoi(argv[1]);
	struct itimerspec ts = { 0 };
	struct epoll_event ev, events[MAX_EVENT];
	int ret = 0;
	int nfd = 0;
	int ep = 0;
	int i = 0;
	unsigned ntimeout = 0;
	uint64_t tmp = 0;

	int tfd = timerfd_create(CLOCK_MONOTONIC, 0);

	//int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	ts.it_interval.tv_sec = timeout;
	ts.it_value.tv_sec = timeout;
	timerfd_settime(tfd, 0, &ts, NULL);
	printf("start a periodic timer, timeout: %d\n", timeout);
	//fcntl(tfd, F_SETFL, O_NONBLOCK);
#if 0
	while (1)
	{
		ret = read(tfd, &tmp, sizeof(tmp));
		printf("read %d, %ld\n", ret, tmp);
	}
#endif
#if 1
	ep = epoll_create(10);
	if (ep == -1)
		goto EXIT;
	ev.events = EPOLLIN | EPOLLET;
	//ev.events = EPOLLIN;
	ev.data.fd = tfd;
	ret = epoll_ctl(ep, EPOLL_CTL_ADD, tfd, &ev);
	if (ret == -1)
		goto EXIT;
	while (1)
	{
		printf("come here %d\n", __LINE__);
		//nfd = epoll_wait(ep, events, MAX_EVENT, -1);
		nfd = epoll_wait(ep, events, MAX_EVENT, 5000);
		printf("come here %d nfd %d\n", __LINE__, nfd);
		if (nfd > 0)
		{
			printf("come here %d\n", __LINE__);
			for (i = 0; i < nfd; i++)
			{
				if (events[i].data.fd == tfd)
				{
					ntimeout++;
					printf("timer out %d\n", ntimeout);
					//read(tfd, &tmp, sizeof(tmp));
				}
			}
		}
	}
#endif
	//read()
      EXIT:
	return 0;
}
