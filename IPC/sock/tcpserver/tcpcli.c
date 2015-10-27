#include <stdio.h>
#include <netinet/in.h>

#define SEV_IP "172.16.16.25"
#define SEV_PORT 9090
#define LOG(fmt, args...)\
    printf("%s(%d)"fmt"\n", __FUNCTION__, __LINE__, ##args);

int main()
{
    	int iFd = 0;
	int iRet = 0;
	char cReq[] = "Request: give me some data";
	char cResp[128] = {0};
	struct sockaddr_in stSevAddr = {0};
	int iAddrLen = 0;

	/* to create a socket to connect to the server */
	iFd = socket(AF_INET, SOCK_STREAM, 0);
	if (iFd <= 0)
	{
		LOG("create socket failed, errcode %d", iFd);
		return -1;
	}

    	/* send requests to server */
	inet_aton(SEV_IP, &stSevAddr.sin_addr);
	stSevAddr.sin_family = AF_INET;
	stSevAddr.sin_port = htons(SEV_PORT);
	iAddrLen = sizeof(stSevAddr);
	while(1)
	{
	    	LOG("Connect to server %s......", SEV_IP);
		iRet = connect(iFd, (struct sockaddr *)(&stSevAddr), sizeof(stSevAddr));
		if (iRet == 0)
		{
			LOG("Connect to %s successfully.", SEV_IP);
			break;
		}
		sleep(5);
	}

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

    	/* recv the data from server, and print the data to terminal */

	close(iFd);
	return 0;
}
