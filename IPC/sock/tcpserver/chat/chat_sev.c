#include "chat.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define SIGNAL
#define MAX_CLIENT_NUM 128

#ifdef SIGNAL
void terminate(int signum)
{
	int iRet = 0;

	LOG("Process %d Rev TERM signal", getpid());

	iRet = wait(NULL);
}
#endif

int main()
{
	int iFd = 0, i = 0;
	int iSonFd[MAX_CLIENT_NUM] = { 0 };
	int iRet = 0;
	struct sockaddr_in stSevAddr = { 0 };
	struct sockaddr_in stCliAddr = { 0 };
	int iAddrLen = 0;
	int iCliIndex = 0;

	pid_t pid = 0;
	struct sigaction sa;
	pthread_t tid_s;
	pthread_t tid_r;

	/* to create a socket to connect to the server */
	iFd = socket(AF_INET, SOCK_STREAM, 0);
	if (iFd <= 0)
	{
		LOG("create socket failed, errcode %d", iFd);
		return -1;
	}

	stSevAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stSevAddr.sin_family = AF_INET;
	stSevAddr.sin_port = htons(SEV_PORT);
	iAddrLen = sizeof(stSevAddr);

	iRet = bind(iFd, (struct sockaddr *)(&stSevAddr), iAddrLen);
	if (iRet != 0)
	{
		LOG("Bind socket(%d) failed, errcode %d, errreason %s", iFd,
		    iRet, strerror(errno));
		goto EXIT;
	}
	LOG("Socket(%d) bind to addr: %s/%d", iFd,
	    inet_ntoa(stSevAddr.sin_addr), SEV_PORT);

	/* set the socket to LINSEN */
	iRet = listen(iFd, 5);
	if (iRet != 0)
	{
		LOG("Listen err, %d", iRet);
		goto EXIT;
	}

	LOG("Wait client to connect......");

#ifdef SIGNAL
	sa.sa_handler = terminate;
	//sigaction(SIGCHLD, &sa, NULL);//SIGCHLD: child status has changed. 
	sigaction(SIGUSR1, &sa, NULL);	//SIGCHLD: child status has changed. 
#endif
	while (iCliIndex < MAX_CLIENT_NUM)
	{
		iRet = accept(iFd, (struct sockaddr *)(&stCliAddr), &iAddrLen);
		if (iRet > 0)
		{
			iSonFd[iCliIndex] = iRet;
			pid = fork();
			if (pid == 0)
			{	//son process
				LOG("LOG info: %s/%d",
				    inet_ntoa(stCliAddr.sin_addr),
				    ntohs(stCliAddr.sin_port));
				if (pthread_create
				    (&tid_s, NULL, send_func,
				     (void *)(&iSonFd[iCliIndex])))
				{
					LOG("create thread for send failed, reason %s", strerror(errno));
					goto EXIT;
				}

				if (pthread_create
				    (&tid_r, NULL, recv_func,
				     (void *)(&iSonFd[iCliIndex])))
				{
					LOG("create thread for rev failed, reason %s", strerror(errno));
					goto EXIT;
				}

				iRet = pthread_join(tid_r, NULL);
				if (iRet != 0)
				{
					LOG("pthread_join return err %d", iRet);
				}
				iRet = pthread_join(tid_s, NULL);
				if (iRet != 0)
				{
					LOG("pthread_join return err %d", iRet);
				}
				LOG("threads finish");

#ifdef SIGNAL
				pid_t ppid = getppid();

				iRet = kill(ppid, SIGUSR1);
				if (iRet != 0)
				{
					LOG("%s", strerror(errno));
				}
#endif

				exit(0);
			}
		}
		iCliIndex++;
		sleep(5);
	}

      EXIT:
	if (iFd != 0)
	{
		close(iFd);
	}

	for (i = 0; i < iCliIndex; i++)
	{
		if (iSonFd[i])
		{
			close(iSonFd[i]);
		}
	}
	return 0;
}
