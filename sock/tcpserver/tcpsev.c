#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define SIGNAL
#define SEV_IP "172.16.16.25"
#define SEV_PORT 9090
#define LOG(fmt, args...)\
    printf("%s(%d)"fmt"\n", __FUNCTION__, __LINE__, ##args);

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
    	int iFd = 0;
	int iSonFd = 0;
	int iRet = 0;
	char cReq[128] = {0};
	char cResp[128] = "You are a fool";
	struct sockaddr_in stSevAddr = {0};
	struct sockaddr_in stCliAddr = {0};
	int iAddrLen = 0;
	pid_t pid = 0;
	struct sigaction sa;

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
		LOG("Bind socket(%d) failed, errcode %d, errreason %s", iFd, iRet, strerror(errno));
		goto ERR;
	}
	LOG("Socket(%d) bind to addr: %s/%d", iFd, inet_ntoa(stSevAddr.sin_addr), SEV_PORT);

	/* set the socket to LINSEN */
	iRet = listen(iFd, 5);
	if (iRet != 0)
	{
		LOG("Listen err, %d", iRet);
		goto ERR;
	}

	LOG("Wait client to connect......");
#ifdef SIGNAL
	sa.sa_handler = terminate;
	//sigaction(SIGCHLD, &sa, NULL);//SIGCHLD: child status has changed. 
	sigaction(SIGUSR1, &sa, NULL);//SIGCHLD: child status has changed. 
#endif
	while(1)
	{
		iSonFd = accept(iFd, (struct sockaddr *)(&stCliAddr), &iAddrLen);
		if (iSonFd > 0)
		{
		        pid = fork();
			if (pid == 0)
			{
				LOG("Accept a connect from %s/%d", inet_ntoa(stCliAddr.sin_addr), ntohs(stCliAddr.sin_port));
				iRet = recv(iSonFd, cReq, sizeof(cReq), 0);
				if (iRet > 0)
				{
					cReq[iRet] = '\0';
					LOG("Recv data from cli %s", cReq);
					if (strstr(cReq, "Request"))
					{
						iRet = send(iSonFd, cResp, sizeof(cResp), 0);
					}
				}

				#ifdef SIGNAL
			    	pid_t ppid = getppid();
				iRet = kill(ppid, SIGUSR1);
				if (iRet != 0)
				{
					LOG("%s", strerror(errno));
				}
				#endif
				//exit(0);
				goto ERR;
			}
		}
		sleep(5);
	}

    	/* recv the data from server, and print the data to terminal */
ERR:
	close(iFd);
	close(iSonFd);
	return 0;
}
