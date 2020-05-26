#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<linux/sem.h>

int mutexid;

void getsema(int semid, char *str)
{
	union semun arg;
	int semval = 0xffffffff;
	semval = semctl(semid, 0, GETVAL);
	printf("%s: value %d\n", str, semval);
}

int main()
{
	int chld, i, j;
	struct sembuf p, v;
	union semun arg;

	mutexid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	arg.val = 1;
	if (semctl(mutexid, 0, SETVAL, arg) == -1)
		perror("semctl setval error");

	p.sem_num = 0;
	p.sem_op = -1;
	p.sem_flg = SEM_UNDO;

	v.sem_num = 0;
	v.sem_op = 1;
	v.sem_flg = SEM_UNDO;

	while ((chld = fork()) == -1) ;
	if (chld > 0)
	{
		i = 1;
		while (i <= 3)
		{
			sleep(1);
			getsema(mutexid, "DAD");
			semop(mutexid, &p, 1);
			printf("prnt in\n");
			sleep(1);
			printf("prnt out\n");
			semop(mutexid, &v, 1);
			i++;
		}
		wait(0);
		semctl(mutexid, IPC_RMID, 0);
		exit(0);
	}
	else
	{
		j = 1;

		while (j <= 3)
		{
			sleep(1);
			getsema(mutexid, "SON");
			semop(mutexid, &p, 1);
			printf("chid in\n");
			sleep(1);
			printf("chld out\n");
			semop(mutexid, &v, 1);
			j++;
		}
		exit(0);
	}
}
