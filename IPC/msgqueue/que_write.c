#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
int msgwrite(key_t key, char *);
int main()
{
	key_t key;
	char content[60];

	printf("\t请输入要传送到消息队列的内容\n");
	fgets(content, 60, stdin);
	key = ftok("file", 'a');
	if (key != -1)
		msgwrite(key, content);
	return 0;
}

int msgwrite(key_t key, char *content)	/*成功返回0失败返回-1 */
{

	struct msgbuf
	{
		int mtype;
		char mtext[60];
	} msgbuff, msgbuff2;
	int msggid;


	if ((msggid = msgget(key, IPC_CREAT)) == -1)
	{
		printf("%s", (char *)strerror(errno));
		return -1;
	}
	msgbuff.mtype = 1;
	strcpy(msgbuff.mtext, content);
	if (msgsnd(msggid, &msgbuff, 51, IPC_NOWAIT) == -1)
	{
		printf("%s", (char *)strerror(errno));
		return -1;
	}
	msgbuff2.mtype = 2;
	strcpy(msgbuff2.mtext, "OOK");
	if (msgsnd(msggid, &msgbuff2, 51, IPC_NOWAIT) == -1)
	{
		printf("%s", (char *)strerror(errno));
		return -1;
	}
	printf("success\n");
	return 0;
}
