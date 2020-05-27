//pv.hÍ·ÎÄ¼ş
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
 
#define SEMPERM 0600
typedef union _semun {
  int val;
  struct semid_ds *buf;
  ushort *array;
} semun;

int init_sem(key_t semkey);
int p(int semid);
int v(int semid);
int destroysem();
void sem_getval(int semid);
