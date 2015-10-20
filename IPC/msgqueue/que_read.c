#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<errno.h>
int msgread(key_t key);
int main()
{
    key_t key;
    
    
    key = ftok("file",'a');
     msgread(key);
}
int msgread(key_t key)/*成功返回0失败返回-1*/
{
    struct msgbuf
    {
        int mtype;
        char mtext[60];
    }msgbuff;
    struct msqid_ds msgds;/*没有初始化，能够执行msgctl，是不是msgctl里面有分配内存给他*/
    int msggid;
    msggid = msgget(key,IPC_CREAT);
  msgrcv(msggid,&msgbuff,60,2,IPC_NOWAIT);
    printf("\t%s",msgbuff.mtext);
    msgrcv(msggid,&msgbuff,60,1,IPC_NOWAIT);
    printf("\t%s",msgbuff.mtext);
    msgctl(msggid,IPC_STAT,&msgds);
    printf("\t%d\n",msgds.msg_ctime);/*检测msggid函数有没有成功里*/
    
    return 0;
}
