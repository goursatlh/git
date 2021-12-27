#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int NVM_sem_id = -1;
struct sembuf NVM_wait, NVM_post;

#define ARRIS_NVM_SEM_KEY 0x3838 

int  NVM_Lock()
{
    printf("NVM_Lock wants to require sem id %d pid=%d.\n", NVM_sem_id, getpid()); 
    if ((NVM_sem_id < 0) || semop(NVM_sem_id, &NVM_wait, 1) != 0)
    {  
        printf( "[NVRAM] Failed to semop wait[%d]: %s", NVM_sem_id, strerror(errno));
        return -1;
    }
    printf("NVM_Lock requires require sem id %d pid=%d.\n", NVM_sem_id, getpid()); 

    return 0;
}

int NVM_Unlock()
{
    printf("NVM_Unlock wants to release sem id %d pid=%d.\n", NVM_sem_id, getpid()); 
    if ((NVM_sem_id < 0) || semop(NVM_sem_id, &NVM_post, 1) != 0)
    {
        printf( "[NVRAM] Failed to semop post[%d]: %s", NVM_sem_id, strerror(errno));
        return -1;
    }
    printf("NVM_Unlock releases require sem id %d pid=%d.\n", NVM_sem_id, getpid()); 

    return 0;
}

void handle_signal(int signo)
{
    if (signo == SIGUSR1)
    {
        printf("pid %d recv SIGUSR1..\n", getpid());
    }
}

int main()
{
    FILE *fp = NULL;
    unsigned short semval;
	NVM_wait.sem_num = 0;
	NVM_wait.sem_op = -1;
	NVM_wait.sem_flg = SEM_UNDO;
	NVM_post.sem_num = 0;
	NVM_post.sem_op = 1;
	NVM_post.sem_flg = SEM_UNDO;
	 
    signal(SIGUSR1, handle_signal);
	NVM_sem_id = semget(ARRIS_NVM_SEM_KEY ,1, 0666);
	if (NVM_sem_id < 0)
	{
		printf("NVM semaphore open error! %s\n", strerror(errno));
		return -1;
	}
	//semctl(NVM_sem_id , 0 , SETVAL, 1);
    NVM_Lock();
    fp = fopen("/home/wt/code/git/IPC/semaphore/share", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s", "hello world");
        fclose(fp);
    }
    sleep(60);
	NVM_Unlock();

    return 0;
}
