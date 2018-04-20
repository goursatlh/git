#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/file.h>
/* shared memoroy and mutex
 * mmap
 * sem_wait/sem_post
 * flock
 *
 * */


/* You must use 'gcc -o share4 share4.c -lrt' to compile the program to link the realtime lib */


#define SEM_NAME "/mysem"

void error_and_die(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void *mmap_func(int *fd_out)
{
    int r;
	const char *memname = "sample";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);
	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");
	
    r = ftruncate(fd, region_size);
	if (r != 0)
		error_and_die("ftruncate");
	
    void *ptr = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	//close(fd);
    *fd_out = fd;
    return ptr;
}

int main(int argc, char *argv[])
{
	int r;
    int val = 0;
    int *d;
	const char *memname = "sample";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);
    sem_t *sem_p = NULL; 

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");

	r = ftruncate(fd, region_size);
	if (r != 0)
		error_and_die("ftruncate");

	void *ptr = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	close(fd);
    d = (int *)ptr;
    
    sem_unlink(SEM_NAME);
    if (!sem_p)
    {
        sem_p = sem_open(SEM_NAME, O_CREAT, 0666, 1);
        if (sem_p == SEM_FAILED)
            error_and_die("fail to open the sem.\n");
        r = sem_getvalue(sem_p, &val);
        printf("sem init vaule %d, ret %d\n", val, r);
#if 0
        sem_post(sem_p);
        r = sem_getvalue(sem_p, &val);
        printf("sem set vaule to %d, ret %d\n", val, r);
#endif
    }

	pid_t pid = fork();
	if (pid == 0)
	{
        int i = 100;
        r = sem_getvalue(sem_p, &val);
        printf("son sem vaule %d, ret %d\n", val, r);
        sem_wait(sem_p);
		//int *d = (int *)mmap_func(&fd);
        //flock(fd, LOCK_EX);
        while (i-- > 0)
        {
            printf("son: %d\n", *d);
            (*d)++;
        }
        sem_post(sem_p);
        sleep(100);
		exit(0);
	}
	else
	{
		int status;
        int i = 100;
        r = sem_getvalue(sem_p, &val);
        printf("parent sem vaule %d, ret %d\n", val, r);
        sem_wait(sem_p);
		//int *d1 = (int *)mmap_func(&fd);
        //flock(fd, LOCK_EX);
        while (i-- > 0)
        {
            printf("parent: %d\n", *d);
        }
        sem_post(sem_p);
		waitpid(pid, &status, 0);
		//printf("child wrote %#lx\n", *(u_long *) ptr);
	}

	r = munmap(ptr, region_size);
	if (r != 0)
		error_and_die("munmap");
	
    r = shm_unlink(memname);
	if (r != 0)
		error_and_die("shm_unlink");
    
    r = sem_getvalue(sem_p, &val);
    printf("out sem vaule %d, ret %d\n", val, r);
    sem_unlink(SEM_NAME);
	return 0;
}
