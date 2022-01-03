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

int count = 0;
void handler(int signo)
{
    count++;
	printf("handler signo:%d, count %d\n", signo, count);
}

int main()
{
	sigset_t set, oset;
	sigemptyset(&set);
	sigemptyset(&oset);

    signal(2,handler);
	sigaddset(&set, 2); //SIGINT
	sigprocmask(SIG_SETMASK, &set, &oset); //×èÈû2ºÅÐÅºÅ

	sigset_t pending;
	sigemptyset(&pending);

    int count = 0;
	while (1){
		sigpending(&pending); //»ñÈ¡pending
		printPending(&pending); //´òÓ¡pendingÎ»Í¼£¨1±íÊ¾Î´¾ö£©
		sleep(1);
		count++;
		if (count == 30){
			sigprocmask(SIG_SETMASK, &oset, NULL); //»Ö¸´Ôø¾­µÄÐÅºÅÆÁ±Î×Ö
			printf("»Ö¸´ÐÅºÅÆÁ±Î×Ö\n");
		}
	}
	return 0;
}

