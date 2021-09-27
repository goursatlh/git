#include <stdio.h>  
#include <errno.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
  
/* version without signal handling */  
int system_without_signal(const char *cmd_string)  
{  
    pid_t pid;  
    int status = -1;  
  
    if (cmd_string == NULL)  
        return (1);     /* always a command processor with UNIX */  
  
    if ((pid = fork()) < 0) 
    {  
        status = -1;    /* probably out of processes */  
    } 
    else if (pid == 0) 
    {  /* child */  
        printf("[in system_without_signal] son process %ld enter\n", (long)getpid());  
        execl("/bin/sh", "sh", "-c", cmd_string, (char *)0);  
        printf("[in system_without_signal] son process %ld exit\n", (long)getpid());  
        _exit(127); /* execl error */  
    } 
    else 
    {                /* parent */  
        sleep(1);  
        pid_t wait_pid;  
        printf("[in system_without_signal] parent process %ld, son id %ld\n", (long)getpid(), (long)pid);  
        while ((wait_pid = waitpid(pid, &status, 0)) < 0)  // wait and send signal, which one handle first ?
        {  
            printf("[in system_without_signal]: errno = %d(%s)\n", errno, strerror(errno));  
            if (errno != EINTR) 
            {  
                status = -1;    /* error other than EINTR form waitpid() */  
                break;  
            }  
        }  
        printf("[in system_without_signal]: pid = %ld, wait_pid = %ld\n",(long)pid, (long)wait_pid);  
        printf("[in system_without_signal] %x\n", status);  
    }  
  
    return (status);  
}  
