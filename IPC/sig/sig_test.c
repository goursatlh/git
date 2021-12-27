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
	sigprocmask(SIG_SETMASK, &set, &oset); //����2���ź�

	sigset_t pending;
	sigemptyset(&pending);

	while (1){
		sigpending(&pending); //��ȡpending
		printPending(&pending); //��ӡpendingλͼ��1��ʾδ����
		sleep(1);
	}
	return 0;
}

