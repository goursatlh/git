#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define SEV_IP "172.16.16.25"
#define SEV_PORT 9090
#define MAX_LEN 128

#define LOG(fmt, args...)\
    printf("%s(%d)"fmt"\n", __FUNCTION__, __LINE__, ##args);

int flag = 0;

void *send_func(void *arg)
{
	char send_msg[MAX_LEN] = { 0 };
	char *psend = NULL;
	int iRet = 0;
	int iFd = *((int *)arg);

	LOG("send thread start") while (1)
	{
		printf("$: ");
		psend = gets(send_msg);
		if (psend)
		{
			if (!strcmp(psend, "quit"))
			{
				flag = 1;
				LOG("Goodbye, send exit.");
				return;
			}
		}

		iRet = send(iFd, send_msg, sizeof(send_msg), EWOULDBLOCK);
		if (iRet <= 0)
		{
			LOG("send msg failed, msg: %s", send_msg);
		}

		if (flag == 1)
		{
			return;
		}
	}
	return;
}

void *recv_func(void *arg)
{
	char recv_msg[MAX_LEN] = { 0 };
	int iRet = 0;
	int iFd = *((int *)arg);
	fd_set rset;
	struct timeval time = { 0 };

	LOG("recv thread start") while (1)
	{
		FD_SET(iFd, &rset);
		time.tv_sec = 5;
		iRet = select(iFd + 1, &rset, NULL, NULL, &time);
		if (iRet > 0)
		{
			if (FD_ISSET(iFd, &rset))
			{
				iRet = recv(iFd, recv_msg, sizeof(recv_msg), 0);
				if (iRet > 0)
				{
					recv_msg[iRet] = '\0';
					printf("\n$: %s\n", recv_msg);
				}
				else if (iRet == 0)
				{
					LOG("Clinet has disconnected, close this session");
					flag = 1;
					return;
				}
			}
		}
		if (flag == 1)
		{
			LOG("Goodbye, recv exit.");
			return;
		}
	}
	return;
}
