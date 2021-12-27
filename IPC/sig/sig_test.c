#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void printPending(sigset_t *pending)
{
	int i = 1;
	for (i = 1; i <= 31; i++){
		if (sigismember(pending, i)){
			printf("1 ");
		}
		else{
			printf("0 ");
		}
	}
	printf("\n");
}

int main()
{
	sigset_t set, oset;
	sigemptyset(&set);
	sigemptyset(&oset);

	sigaddset(&set, 2); //SIGINT
	sigprocmask(SIG_SETMASK, &set, &oset); //阻塞2号信号

	sigset_t pending;
	sigemptyset(&pending);

	while (1){
		sigpending(&pending); //获取pending
		printPending(&pending); //打印pending位图（1表示未决）
		sleep(1);
	}
	return 0;
}

