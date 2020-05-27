/*
flow:
1, semget // create a new semaphore
2, semctl // SETVAL: initalize the semaphore
3, semop  // p.v operations
4, semctl // IPC_RMID: remove the semaphore
5, ipcs -a // display the semaphores in the system
6, ipcrm -s semid // delete the semaphore by semid
*/
 
#include "pv.h"
 
int init_sem(key_t semkey)
{
  int status=0,semid;                    //�ź�����ʶ��semid

  if ((semid=semget(semkey,1,SEMPERM|IPC_CREAT|IPC_EXCL))==-1)
  {
    if (errno==EEXIST)          //EEXIST���ź������Ѿ����ڣ��޷�����
    {
      semid=semget(semkey,1,0);      //����һ���ź���
      printf("init: sem is already created, semid %d\n", semid);
    }
  }
  else
  {
    semun arg;
    arg.val=1;                                        //�ź����ĳ�ֵ
    printf("init: create a new sem, semid %d\n", semid);
    sem_getval(semid);
    status=semctl(semid,0,SETVAL,arg);      //�����ź������е�һ���������ź�����ֵ��
    //v(semid);
    sem_getval(semid);
  }

  if (semid==-1||status==-1)
  {
    printf("initsem failed");
    return(-1);
  }
  /*all ok*/
  return(semid);
}
 
int p(int semid)
{
  struct sembuf p_buf;
 
  p_buf.sem_num=0;
  p_buf.sem_op=-1;        //�ź�����1��ע����һ�е�1ǰ���и�����
  p_buf.sem_flg=SEM_UNDO;
  //p_buf.sem_flg=0;
  
  //p_buf = {0,-1,SEM_UNDO};
  if (semop(semid, &p_buf, 1)==-1)   
  {
    printf("p(semid)failed, pid %d", getpid());
    exit(1);
  }
  return(0);
}
 
int v(int semid)
{
  struct sembuf v_buf;
 
  v_buf.sem_num=0;
  v_buf.sem_op=1;    //�ź�����1
  v_buf.sem_flg=SEM_UNDO;
  //v_buf.sem_flg=0;
  
  if (semop(semid, &v_buf, 1)==-1)
  {
    printf("v(semid)failed, pid %d", getpid());
    exit(1);
  }
  return(0);
}
 
int destroy_sem(int semid){
 // fprintf(stderr, "Failed to delete semaphore\n");
 return semctl(semid,0,IPC_RMID);  //ɾ�������ź���ֵ��IPC_RMID��ɾ������
}

void sem_getval(int semid)
{   
    int semval = 0xffffffff;
    semval = semctl(semid, 0, GETVAL);
    printf("semid: %d, pid %d, sem value %d\n", semid, getpid(), semval);
}
