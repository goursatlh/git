#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFSIZE  8192
#define DEFAULT_PORT  5320

enum
{ CMD_NAME, DST_IP, DST_PORT };


int execute(char *command, char *buf, int bufmax)
{
	FILE *fp;
	int i;

	if ((fp = popen(command, "r")) == NULL)
	{
		perror("popen error\n");
		exit(1);
	}
	else
	{
		i = 0;
		while ((buf[i] = fgetc(fp) != EOF) && i < bufmax - 1)
			i++;
		pclose(fp);
	}
	return i;
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server;
	unsigned long dst_ip;
	int port;
	int s;
	int n;
	char buf[BUFSIZE];
	char cmd[BUFSIZE];

	struct timeval tv;
	fd_set readfd;

	if (argc != 2 && argc != 3)
	{
		fprintf(stderr, "Usege:%s hostname [port]\n", argv[0]);
		exit(1);
	}

	if ((dst_ip = inet_addr(argv[DST_IP])) == INADDR_NONE)
	{
		struct hostent *he;

		if ((he = gethostbyname(argv[DST_IP])) == NULL)
		{
			fprintf(stderr, "gethostbyname error");
			exit(1);
		}
		memcpy((char *)&dst_ip, (char *)he->h_addr, sizeof(dst_ip));
	}

	if (argc == 3)
	{
		if ((port = atoi(argv[DST_PORT])) == 0)
		{
			struct servent *se;

			if ((se = getservbyname(argv[DST_PORT], "TCP")) != NULL)
				port = (int)ntohs((u_short) se->s_port);
			else
			{
				fprintf(stderr, "getserverbyname error\n");
				exit(1);
			}

		}
	}
	else
	{
		port = DEFAULT_PORT;
	}
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error\n");
		exit(1);
	}

	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = dst_ip;
	server.sin_port = htons(port);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect error");
		exit(1);
	}
	printf(" Connected to '%s' \n", inet_ntoa(server.sin_addr));

	while (1)
	{
		tv.tv_sec = 600;
		tv.tv_usec = 0;

		FD_ZERO(&readfd);
		FD_SET(0, &readfd);
		FD_SET(s, &readfd);

		if ((select(s + 1, &readfd, NULL, NULL, &tv)) <= 0)
		{
			fprintf(stderr, "\nTimeout\n");
			break;
		}
		if (FD_ISSET(0, &readfd))
		{
			if ((n = read(0, buf, BUFSIZE - 1)) <= 0)
			{
				break;
			}
			buf[n] = '\0';
			sscanf(buf, "%s", cmd);
			if (strcmp(cmd, "quit") == 0)
				break;
			if (send(s, buf, n, 0) <= 0)
				break;
		}
		if (FD_ISSET(s, &readfd))
		{
			if ((n = recv(s, buf, BUFSIZE - 1, 0)) <= 0)
			{
				fprintf(stderr, "connection closed.\n");
				exit(1);
			}
			buf[n] = '\0';
			printf(buf);
			fflush(stdout);
		}
	}
	strcpy(buf, "quit");
	send(s, buf, n, 0);
	close(s);

	return 1;
}
