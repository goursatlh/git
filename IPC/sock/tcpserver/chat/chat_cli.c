#include "chat.h"

int main()
{
    	int iFd = 0;
	int iRet = 0;
	struct sockaddr_in stSevAddr = {0};
	int iAddrLen = 0;
	pthread_t tid_s;
	pthread_t tid_r;

	/* to create a socket to connect to the server */
	iFd = socket(AF_INET, SOCK_STREAM, 0);
	if (iFd <= 0)
	{
		LOG("create socket failed, errcode %d\n", iFd);
		return -1;
	}

    	/* send requests to server */
	inet_aton(SEV_IP, &stSevAddr.sin_addr);
	stSevAddr.sin_family = AF_INET;
	stSevAddr.sin_port = htons(SEV_PORT);
	iAddrLen = sizeof(stSevAddr);
	while (1)
	{
	    	LOG("Connect to server %s......\n", SEV_IP);
		iRet = connect(iFd, (struct sockaddr *)(&stSevAddr), sizeof(stSevAddr));
		if (iRet == 0)
		{
			LOG("Connect to %s successfully.\n", SEV_IP);
			break;
		}
		sleep(5);
	}

	//start two threads for send and recv.
	if (pthread_create(&tid_s, NULL, send_func, (void *)(&iFd)))
	{
		LOG("create thread for send failed, reason %s", strerror(errno));
		goto EXIT;
	}

	if (pthread_create(&tid_r, NULL, recv_func, (void *)(&iFd)))
	{
		LOG("create thread for rev failed, reason %s", strerror(errno));
		goto EXIT;
	}

	pthread_join(tid_r, NULL);
	pthread_join(tid_s, NULL);

EXIT:
	close(iFd);
	return 0;
}
