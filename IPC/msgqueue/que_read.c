#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<errno.h>
int msgread(key_t key);
int main()
{
	key_t key;
	key = ftok("file", 'a');
	msgread(key);
}

int msgread(key_t key)		/*�ɹ�����0ʧ�ܷ���-1 */
{
	struct msgbuf
	{
		int mtype;
		char mtext[60];
	} msgbuff;
	struct msqid_ds msgds;	/*û�г�ʼ�����ܹ�ִ��msgctl���ǲ���msgctl�����з����ڴ���� */
	int msggid;

	msggid = msgget(key, IPC_CREAT);
    printf("read msg id %d, key %d\n", msggid, key);
	msgctl(msggid, IPC_STAT, &msgds);
	printf("msg queue info: \n");	
	printf("msg num %d\n", msgds.msg_qnum);	
	msgrcv(msggid, &msgbuff, 60, 2, IPC_NOWAIT);
	printf("%s", msgbuff.mtext);
	msgrcv(msggid, &msgbuff, 60, 1, IPC_NOWAIT);
	printf("%s", msgbuff.mtext);
	msgctl(msggid, IPC_STAT, &msgds);
	printf("msg queue info: \n");	
	printf("msg num %d\n", msgds.msg_qnum);	

	return 0;
}
