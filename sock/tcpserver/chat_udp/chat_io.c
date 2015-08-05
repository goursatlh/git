#include "chat.h"

int flag = 0;

void *send_func(void *arg)
{
    	char send_msg[MAX_LEN] = {0};
    	char *psend = NULL;
	int iRet = 0;
	thread_s *threads = (thread_s *)arg;
	int iFd = threads->fd;
	
	LOG("send thread start")
	while (1)
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

		iRet = sendto(iFd, send_msg, sizeof(send_msg), 0, 
			     (struct sockaddr *)(threads->addr), sizeof(struct sockaddr));
		if (iRet <= 0)
		{
			LOG("send msg failed, msg: %s, err reason: %s", send_msg, strerror(errno));
		}
	}
	return;
}

void *recv_func(void *arg)
{
    	char recv_msg[MAX_LEN] = {0};
	int iRet = 0;
	fd_set rset;
	struct timeval time = {0};
	thread_s *thread = (thread_s *)arg;
	int iFd = thread->fd;

	LOG("recv thread start")
	while(1)
	{
	//	FD_ZERO(&rset);
	    	FD_SET(iFd, &rset);
		time.tv_sec = 5;
	    	
		iRet = select(iFd+1, &rset, NULL, NULL, &time);
		if (iRet > 0)
		{
		    	LOG("fd is readable, %d", iRet);
		    	if (FD_ISSET(iFd, &rset))
			{
				iRet = recvfrom(iFd, recv_msg, sizeof(recv_msg), 0, NULL, NULL);
				if (iRet > 0)
				{
					recv_msg[iRet] = '\0';
					printf("\n$: %s\n", recv_msg);
				}
			}
		}
		else
		{
			//LOG("select return %d", iRet);
		}
		if (flag == 1)
		{
			LOG("Goodbye, recv exit.");
			return;
		}
	}
	return;
}
