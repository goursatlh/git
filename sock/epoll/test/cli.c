#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>

#define SEV_IP "172.16.16.25"
#define SEV_PORT 9090
#define LOG(fmt, args...)\
    printf("%s(%d)"fmt"\n", __FUNCTION__, __LINE__, ##args);

int main(int argc, char **argv)
{
    	int iFd = 0;
	int iRet = 0;
	char cReq[] = "Request: give me some data";
	char cResp[128] = {0};
	struct sockaddr_in stSevAddr = {0};
	int iAddrLen = 0;
	short port = 0;
	int clientnum, sendtimes, senddata;
	pid_t pid = -255;
	int i = 0, j = 0;

	if (argc != 6)
	{
		LOG("bad input. usgage: cli ipaddr port clientnum sendtimes senddata.");
		return 0;
	}
	LOG("Input commonds: %s %s %s %s %s %s", argv[0], argv[1], argv[2], argv[3], argv[4],argv[5]);
	
	port = atoi(argv[2]);
	clientnum = atoi(argv[3]);
	sendtimes = atoi(argv[4]);
	senddata  = atoi(argv[5]);

	for (i = 0; i < clientnum; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			/* to create a socket to connect to the server */
			iFd = socket(AF_INET, SOCK_STREAM, 0);
			if (iFd <= 0)
			{
				LOG("create socket failed, errcode %d", iFd);
				return -1;
			}

			/* send requests to server */
			inet_aton(argv[1], &stSevAddr.sin_addr);
			stSevAddr.sin_family = AF_INET;
			stSevAddr.sin_port = htons(port);
			iAddrLen = sizeof(stSevAddr);
			while(1)
			{
				LOG("Connect to server %s......", argv[1]);
				iRet = connect(iFd, (struct sockaddr *)(&stSevAddr), sizeof(stSevAddr));
				if (iRet == 0)
				{
					LOG("Connect to %s successfully.", SEV_IP);
					break;
				}
				sleep(5);
			}

			for (j = 0; j < sendtimes; j++)
			{    
				iRet = send(iFd, cReq, sizeof(cReq), 0);
				if (iRet == sizeof(cReq))
				{
					iRet = recv(iFd, cResp, sizeof(cResp), 0);
					if (iRet > 0)
					{
						cResp[iRet] = '\0';
						LOG("Data from server: ");
						LOG("%s", cResp);
					}

				}
			}
			/* recv the data from server, and print the data to terminal */
			goto EXIT;
		}
	}

	while(wait(NULL)>0);

	LOG("Wait err %s", strerror(errno));

EXIT:	
	close(iFd);
	return 0;
}
