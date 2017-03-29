#if 0
#include <stdio.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MB 1024*1024

int main()
{

    int pid;

    printf("new process PID=%d\n", getpid());
    pid = fork();
    if(pid==0)
    {   
        /* child */
        printf("new process PID=%d\n", getpid());
        char * p = malloc(512*MB);
        memset(p,0,512*MB);
        exit(0);
    }   
    else
    {   
        /* parent */
        sleep(60);
    }   
}   
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <signal.h>

static int do_abort = 0;

void handle_signal(int signo)
{
    if (signo == SIGHUP)
    { 
        printf("child recv SIGHUP..\n");
        do_abort = 1;
    }
}

int main(void)
{
    pid_t pid;
    pid = fork();
    char *p = NULL;

    if (pid == 0) // child
    {
        signal(SIGHUP, handle_signal);
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        while(!do_abort) {
            sleep(1);
            printf("in child...\n");
        }
        printf("child exit...\n");
        return 0;
    }
    else // parent
    {
        int times = 5;
        while(times-- > 0)
        {
            sleep(1);
            if (times == 3)
            {
                printf("memcpy ...\n");
                memcpy(p, "Hello", 5);
            }
            printf("in parent.\n");
        }

        printf("parent exit..\n");
        return 0;
    }

    return 0;
}


