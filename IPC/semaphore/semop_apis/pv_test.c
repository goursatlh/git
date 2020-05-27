//testsem.c  ������ʹ��PV����ʵ���������̵Ļ���
#include "pv.h"
#include <errno.h>
void handlesem(key_t skey);
int semid;

void getsema(int semid)
{   
    int semval = 0xffffffff;
    semval = semctl(semid, 0, GETVAL);
    printf("semid: %d, pid %d, sem value %d\n", semid, getpid(), semval);
}

void handlesem(key_t skey)
{
  int sleep_s=30;
  pid_t pid=getpid();
  
  if ((semid=init_sem(skey))<0)
    exit(1);
  printf("process %d before the crical area\n",pid);
  getsema(semid);
  p(semid);                                      //���̽����ٽ���Դ�����ź�������1
  printf("process %d holding the crical area, %ds\n",pid, sleep_s);
 
  /*in real life do something interesting */
  sleep(sleep_s);
  //printf("%d\n", sleep_s/0);
  printf("process %d after the crical area\n", pid);
  v(semid);                                //�����˳��ٽ���Դ�����ź�����1
  getsema(semid);
  printf("process %d release from crical area\n", pid);
  exit(0);
}

int main()
{
  key_t semkey=0x200;
  int i;
  for (i=0;i<2;i++)
  {
    if (fork()==0)           //�����̸������3���ӽ���
      handlesem(semkey);  //�ӽ����в�ִ��handlesem��������exit��
  }

  wait(NULL); // wait one of the sub processes 
  if (destroy_sem(semid)<0)
  {
    //perror("semctl error, pid %d, err: %s", getpid(), strerror(errno));
    printf("semctl error, pid %d exit\n", getpid());
    exit(1);
  }
}



