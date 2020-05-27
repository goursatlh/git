//testsem.c  主程序，使用PV操作实现三个进程的互斥
#include "pv.h"
#include <errno.h>
void handlesem(key_t skey);
int semid;

void handlesem(key_t skey)
{
  int sleep_s=120;
  pid_t pid=getpid();
  
  if ((semid=init_sem(skey))<0)
    exit(1);
  printf("process %d before the crical area\n",pid);
  sem_getval(semid);
  p(semid);                                      //进程进入临界资源区，信号量减少1
  printf("process %d holding the crical area, %ds\n",pid, sleep_s);
 
  /*in real life do something interesting */
  sleep(sleep_s);
  //printf("%d\n", sleep_s/0);
  printf("process %d after the crical area\n", pid);
  v(semid);                                //进程退出临界资源区，信号量加1
  sem_getval(semid);
  printf("process %d release from crical area\n", pid);
  return;
}

int main()
{
  key_t semkey=0x200;
  handlesem(semkey);

  if (destroy_sem(semid)<0)
  {
    printf("semctl error, pid %d exit\n", getpid());
    exit(1);
  }
  return 0;
}



