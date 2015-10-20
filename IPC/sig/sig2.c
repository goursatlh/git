#include "../include/apue.h"
#include <pwd.h>

char *pcAA = NULL;

int i = 0;

static void my_alarm(int signo)
{
	struct passwd *rootptr;

	printf("in signal handler\n");

	i -= 5;
	alarm(1);
}

int main(void)
{
	struct passwd *ptr;

	pcAA = "Fuck you!!";
	signal(SIGALRM, my_alarm);
	alarm(1);
	for (i; i < 30; i++)
	{
		sleep(5);
		printf("the string is %d\n", i);
	}
}
