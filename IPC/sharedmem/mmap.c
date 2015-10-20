#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <sys/mman.h> 
#define NLOOPS          10 
#define SIZE            sizeof(long)         /* size of shared memory area */ 


static int pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
		printf("pipe error");
}

void TELL_PARENT(pid_t pid)
{
	if (write(pfd2[1], "c", 1) != 1)
		printf("write error");
}

void WAIT_PARENT(void)
{
	char c;

	if (read(pfd1[0], &c, 1) != 1)
		printf("read error");

	if (c != 'p')
		printf("WAIT_PARENT: incorrect data");
}

void TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1], "p", 1) != 1)
		printf("write error");
}

void WAIT_CHILD(void)
{
	char c;

	if (read(pfd2[0], &c, 1) != 1)
		printf("read error");

	if (c != 'c')
		printf("WAIT_CHILD: incorrect data");
}
static int update(long *ptr) 
{
    printf("pid %d, ptr vaule %d\n", getpid(), *ptr);
    return((*ptr)++);        /* return value before increment */ 
} 

int main(void) 
{ 
    int        fd, i, counter; 
    pid_t      pid; 
    void       *area; 

    if ((fd = open("/dev/zero", O_RDWR)) < 0) 
        printf("open error"); 

    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) 
        printf("mmap error"); 
   
    close(fd);          /* can close /dev/zero now that it’s mapped */ 

    TELL_WAIT(); 

    if ((pid = fork()) < 0) { 
        printf("fork error"); 
    } else if (pid > 0) {                  /* parent */ 
        for (i = 0; i < NLOOPS; i += 2) { 
            if ((counter = update((long *)area)) != i) 
                printf("parent: expected %d, got %d", i, counter); 

            TELL_CHILD(pid); 
            WAIT_CHILD(); 
        } 
    } else {                               /* child */ 
        for (i = 1; i < NLOOPS + 1; i += 2) { 
            WAIT_PARENT(); 

            if ((counter = update((long *)area)) != i) 
                printf("child: expected %d, got %d", i, counter); 

            TELL_PARENT(getppid()); 
        } 
    } 

    exit(0); 
} 
