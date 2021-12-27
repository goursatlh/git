#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>          
#include <sys/stat.h>     
#include <semaphore.h>


sem_t       *DB_sem_ptr;

#define  ARRIS_SEM_DB_NAME "/WALTER"
#define ARRIS_SEM_PERMISSIONS       0666
#define ARRIS_SEM_INIT_TAKEN        0 

int main()
{
    FILE *fp = NULL;

	DB_sem_ptr = sem_open( ARRIS_SEM_DB_NAME, 0);
    //DB_sem_ptr = sem_open( ARRIS_SEM_DB_NAME, O_CREAT, ARRIS_SEM_PERMISSIONS, ARRIS_SEM_INIT_TAKEN);
	if( DB_sem_ptr == SEM_FAILED )
	{
		printf( "DB semaphore access error, %s\n", strerror(errno));
		return -1;
	}	
    //sem_post( DB_sem_ptr ); 

    printf( "sem_wait before pid=%d\n", getpid());
    sem_wait(DB_sem_ptr);
    printf( "sem_wait after  pid=%d\n", getpid());
    fp = fopen("/home/wt/code/git/IPC/semaphore/share", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s", "hello world");
        fclose(fp);
    }
    sleep(60);
    printf( "sem_post before pid=%d\n", getpid());
    sem_post(DB_sem_ptr);
    printf( "sem_post after  pid=%d\n", getpid());
    return 0;
}
