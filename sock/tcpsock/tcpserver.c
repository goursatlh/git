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
#define FORK_SERVER
enum
{ CMD_NAME, SRC_PORT };

int execute(char *command, char *buf, int bufmax);

int execute(char *command, char *buf, int bufmax)
{
	FILE *fp;
	int i;

	if ((fp = popen(command, "r")) == NULL)
	{
		perror(command);
		i = sprintf(buf, "server error:'%s' cannot execute.\n",
			    command);
	}
	else
	{
		i = 0;
		fprintf(stdout, "server execute:'%s' execute ok .\n", command);
#if 0
		while ((buf[i] = fgetc(fp) != EOF) && i < bufmax - 1)
		{
			printf("buf[%d]: %c\n", i, buf[i]);
			i++;
		}
#endif
		i = fread(buf, 1, bufmax, fp);

		printf(buf);
		pclose(fp);
	}
	return i;
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int len;
	int port;
	int s;
	int s0;
	int cn;
	int sn;
	int rn;
	int pid;
	char cmd1[BUFSIZE];
	char cmd2[BUFSIZE];
	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE] = { 0 };

	if (argc == 2)
	{
		if ((port = atoi(argv[SRC_PORT])) == 0)
		{
			struct servent *se;

			if ((se = getservbyname(argv[SRC_PORT], "tcp")) != NULL)
				port = (int)ntohs((u_char) se->s_port);
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

	printf("start a tcp server port %d\n", port);
	if ((s0 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("SOCKET");
		exit(1);
	}
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(s0, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("bind error");
		exit(1);
	}

	listen(s0, 5);

	printf("wait client to connecting............\n");

	while (1)
	{
		len = sizeof(client);
		if ((s = accept(s0, (struct sockaddr *)&client, &len)) < 0)
		{
			perror("accept error");
			exit(1);
		}

		printf("connected from '%s' \n", inet_ntoa(client.sin_addr));

#ifdef FORK_SERVER
		if ((pid = fork()) != 0)
		{
			close(s);
			continue;
		}
		printf
		    ("sub process(%d) has been created to process client request\n",
		     getpid());
		close(s0);
#endif

		while (1)
		{
			int i = 0;

			sn = sprintf(send_buf, "tcp->");
			send(s, send_buf, sn, 0);

		      receive:
			if ((rn = recv(s, &recv_buf[i], 1, 0)) < 0)
				break;

			if (recv_buf[i] != '\n')
			{
				i++;
				if (i < BUFSIZE - 1)
					goto receive;
			}
			recv_buf[i] = '\0';
			printf("receive '%s' \n", recv_buf);

			if ((cn = sscanf(recv_buf, "%s%s", cmd1, cmd2)) <= 0)
				continue;
			else if (cn == 2 && strcmp(cmd1, "show") == 0)
			{
				printf("cmd2:%s\n", cmd2);
				if (strcmp(cmd2, "route") == 0)
				{
					memset(send_buf, 0, BUFSIZE);
					sn = execute("/bin/netstat -rn",
						     send_buf, BUFSIZE);
				}
				else
					sn = sprintf(send_buf,
						     "parameter error '%s'\n"
						     "show[route | apr | tcp | nic]\n",
						     cmd2);
			}
			else
				sn = sprintf(send_buf, "command error '%s'\n",
					     cmd1);

			if (sn == 0)
				sn = sprintf(send_buf, "\n");

			printf(send_buf);
			if (send(s, send_buf, sn, 0) < 0)
				break;
		}

		printf("connection closed.\n");
		close(s);
	}
	close(s0);
	return 0;
}
