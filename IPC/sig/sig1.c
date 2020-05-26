#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>

static void my_alarm(int signo)
{
	struct passwd *rootptr;

	printf("in signal handler %d\n", getpid());
	if ((rootptr = getpwnam("root")) == NULL)
		printf("getpwnam(root) error\n");
	printf("signal handler here %d\n", __LINE__);
	alarm(1);
	printf("out signal handler %d\n", getpid());
}

int main(void)
{
	struct passwd *ptr;

	printf("in main handler %d\n", getpid());
	signal(SIGALRM, my_alarm);
	alarm(1);
	printf("main handler here %d\n", __LINE__);
	for (;;)
	{
	    printf("main handler here %d\n", __LINE__);
		if ((ptr = getpwnam("wt")) == NULL)
			printf("getpwnam error\n");
		if (strcmp(ptr->pw_name, "wt") != 0)
			printf("return value corrupted!, pw_name = %s\n",
			       ptr->pw_name);
	}
}
