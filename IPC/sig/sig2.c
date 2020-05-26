
/****************************************************************************************
 * This file is designed to test the deadlock in signal handler function.
 * ctime() is an un-reentrant function.
 * Only async-signal-safe functions can safely be called from within a signal handler.
 ****************************************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

time_t now;
static void my_alarm(int signo)
{
	printf("in signal handler %d\n", getpid());
    ctime(&now);  // deadlock 
    printf("Current Time and Date:\t%s",ctime(&now));
	alarm(1);
}

int main(void)
{
    int i = 0;
	printf("in main handler %d\n", getpid());
	signal(SIGALRM, my_alarm);
	alarm(1);
	for (i=0; i < 30000000; i++)
	{
	    printf("in main handler %d i %d\n", getpid(), i);
        ctime(&now);
        printf("Current Time and Date:\t%s",ctime(&now));
	}
    return 0;
}
