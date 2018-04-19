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

/* You must use 'gcc -o share4 share4.c -lrt' to compile the program to link the realtime lib */
void error_and_die(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void *mmap_func()
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
	close(fd);
    return ptr;
}

int main(int argc, char *argv[])
{
	int r;
#if 0
	const char *memname = "sample";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);
    sem_t *sem_p = NULL; 

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");

	r = ftruncate(fd, region_size);
	if (r != 0)
		error_and_die("ftruncate");

    //sem_p = sem_open("mysem", O_CREAT, 0666, 0);
	void *ptr = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	close(fd);
#endif

	pid_t pid = fork();
	if (pid == 0)
	{
        int i = 100;
        sem_t *sem_p = NULL; 
        if (!sem_p)
        {
            sem_p = sem_open("mysem", O_CREAT, 0666, 0);
            if (sem_p == SEM_FAILED)
                error_and_die("fail to open the sem.\n");
        }
        
        sem_wait(sem_p);
		int *d = (int *)mmap_func();
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
        sem_t *sem_p = NULL; 
        
        if (!sem_p)
        {
             sem_p = sem_open("mysem", 0);
             if (sem_p == SEM_FAILED)
                error_and_die("fail to open the sem.\n");
        }
        sem_wait(sem_p);
		int *d1 = (int *)mmap_func();
        while (i-- > 0)
        {
            printf("parent: %d\n", *d1);
        }
        sem_post(sem_p);
		waitpid(pid, &status, 0);
		//printf("child wrote %#lx\n", *(u_long *) ptr);
	}

	//r = munmap(ptr, region_size);
	if (r != 0)
		error_and_die("munmap");
#if 0
	r = shm_unlink(memname);
	if (r != 0)
		error_and_die("shm_unlink");
#endif
	return 0;
}
