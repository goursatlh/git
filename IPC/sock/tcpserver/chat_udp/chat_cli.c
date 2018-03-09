#include "chat.h"

int main(int argc, char **argv)
{
	int fd = 0;
	int ret = 0;
	struct sockaddr_in l_addr = { 0 };
	struct sockaddr_in r_addr = { 0 };
	int addr_len = 0;
	unsigned short lport = 0;
	unsigned short rport = 0;
	pthread_t tid_s;
	pthread_t tid_r;
	thread_s threads;

	if (argc != 4)
	{
		LOG("Input error, Useage: cli peer-ip peer-port local_port");
		return 0;
	}
	else
	{
		LOG("%s %s %s %s", argv[0], argv[1], argv[2], argv[3]);
	}

	/* to create a socket to connect to the server */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd <= 0)
	{
		LOG("create socket failed, errcode %d\n", fd);
		return -1;
	}

	/* bind sock info */
	rport = atoi(argv[2]);
	lport = atoi(argv[3]);
	l_addr.sin_family = AF_INET;
	l_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	l_addr.sin_port = htons(lport);
	addr_len = sizeof(struct sockaddr_in);

	ret = bind(fd, (struct sockaddr *)(&l_addr), addr_len);
	if (ret != 0)
	{
		LOG("Bind socket(%d) failed, errcode %d, errreason %s", fd, ret,
		    strerror(errno));
		goto EXIT;
	}

	/* set peer info to send data */
	r_addr.sin_family = AF_INET;
	inet_aton(argv[1], &r_addr.sin_addr);
	r_addr.sin_port = htons(rport);

	while (1)
		sleep(5);

#if 0
	threads.fd = fd;
	threads.addr = &r_addr;
	//start two threads for send and recv.
	if (pthread_create(&tid_s, NULL, send_func, (void *)(&threads)))
	{
		LOG("create thread for send failed, reason %s",
		    strerror(errno));
		goto EXIT;
	}

	if (pthread_create(&tid_r, NULL, recv_func, (void *)(&threads)))
	{
		LOG("create thread for rev failed, reason %s", strerror(errno));
		goto EXIT;
	}

	pthread_join(tid_r, NULL);
	pthread_join(tid_s, NULL);
#endif
      EXIT:
	close(fd);
	return 0;
}
