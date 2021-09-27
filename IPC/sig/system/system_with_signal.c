#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>  
#include <signal.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
  
/* with appropriate signal handling */  
int system_with_signal(const char *cmd_string)  
{  
    pid_t       pid;  
    int         status;  
    struct      sigaction ignore, saveintr, savequit;  
    sigset_t    chld_mask, save_mask;  
      
    if (cmd_string == NULL)  
        return (1);     /* always a command processor with UNIX */  
  
    /* ignore signal SIGINT and SIGQUIT */  
    ignore.sa_handler = SIG_IGN;  
    ignore.sa_flags = 0;  
    sigemptyset(&ignore.sa_mask);  
    if (sigaction(SIGINT, &ignore, &saveintr) < 0)   
        return (-1);  
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0)  
        return (-1);  
  
    /* block SIGCHLD and save current signal mask */  
    sigemptyset(&chld_mask);  
    sigaddset(&chld_mask, SIGCHLD);  
    if (sigprocmask(SIG_BLOCK, &chld_mask, &save_mask) < 0)  
        return (-1);  
  
    if ((pid = fork()) < 0) {  
        status = -1;    /* probably out of processes */  
    } else if (pid == 0) {      /* child */  
        /* restore previous signal actions & reset signal mask */  
        sigaction(SIGINT, &saveintr, NULL);  
        sigaction(SIGQUIT, &savequit, NULL);  
        sigprocmask(SIG_SETMASK, &save_mask, (sigset_t *)NULL);  
  
        execl("/bin/sh", "sh", "-c", cmd_string, (char *)0);  
        _exit(127);  
    } else {                    /* parent */  
        int wait_pid;  
    //  sleep(10);  /* */  
        while ((wait_pid = waitpid(pid, &status, 0)) < 0) {  
            printf("[in system_with_signal]: errno = %d(%s)\n",   
                                        errno, strerror(errno));  
            if (errno != EINTR) {  
                status = -1;    /* error other than EINTR from waitpid() */  
                break;  
            }  
        }  
        printf("[in system_with_signal]: pid = %ld, wait_pid = %ld\n",   
                                        (long)pid, (long)wait_pid);  
        printf("[in system_with_signal] %x\n", status);  
    }  
  
    /* in parent: restore previous signal action & reset signal mask */  
    if (sigaction(SIGINT, &saveintr, NULL) < 0)   
        return (-1);  
    if (sigaction(SIGQUIT, &savequit, NULL) < 0)  
        return (-1);  
    if (sigprocmask(SIG_SETMASK, &save_mask, (sigset_t *)NULL) < 0)  /* */  
        return (-1);  
  
    return (status);  
}  
