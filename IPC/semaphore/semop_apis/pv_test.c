//testsem.c  主程序，使用PV操作实现三个进程的互斥
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
  p(semid);                                      //进程进入临界资源区，信号量减少1
  printf("process %d holding the crical area, %ds\n",pid, sleep_s);
 
  /*in real life do something interesting */
  sleep(sleep_s);
  //printf("%d\n", sleep_s/0);
  printf("process %d after the crical area\n", pid);
  v(semid);                                //进程退出临界资源区，信号量加1
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
    if (fork()==0)           //父进程负责产生3个子进程
      handlesem(semkey);  //子进程中才执行handlesem，做完后就exit。
  }

  wait(NULL); // wait one of the sub processes 
  if (destroy_sem(semid)<0)
  {
    //perror("semctl error, pid %d, err: %s", getpid(), strerror(errno));
    printf("semctl error, pid %d exit\n", getpid());
    exit(1);
  }
}



