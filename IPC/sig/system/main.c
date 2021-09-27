#include <stdio.h>  
#include <errno.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <string.h>  
  
#define SETSIG(sa, sig, fun, flags) \
do {                                \
    sa.sa_handler = fun;            \
    sa.sa_flags = flags;            \
    sigemptyset(&sa.sa_mask);       \
    sigaction(sig, &sa, NULL);      \
} while (0)  
 
extern int system_without_signal(const char *cmd_string);  
extern int system_with_signal(const char *cmd_string);  
  
static void sig_chld(int signo)  
{  
    printf("\nenter SIGCHLD handler\n");  
      
    pid_t pid;  
    int exit_status = -1;  
    int errno_saved = errno;  
    pid = wait(&exit_status);  
    if (pid != -1) 
    {  
        printf("[in sig_chld] reaped %ld child,", (long)pid);  
        printf("wait: %d\n", exit_status);  
    } 
    else 
    { 
        printf("[in sig_chld] wait error: errno = %d(%s)\n\n",   
                                        errno, strerror(errno)); 
    }  
  
    errno = errno_saved;  
    printf("leave SIGCHLD handler\n"); 

    return;
}  
  
int main(int argc, const char *argv[])  
{  
    pid_t pid;  
    struct sigaction sigchld_act;  
  
    SETSIG(sigchld_act, SIGCHLD, sig_chld, 0);  
  
    int status;  
    if ((status = system_without_signal("/bin/ls -l; exit 44")) < 0) 
    //if ((status = system_with_signal("/bin/ls -l; exit 44")) < 0) 
    {  
        printf("system() error(status = %d): \n", status);  
    }  
    printf("system() return: %x\n", status);  
  
    exit(EXIT_SUCCESS);  
}

