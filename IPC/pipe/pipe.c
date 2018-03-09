#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int n;
	int fd[2];
	pid_t pid;
	char line[256];

	if (pipe(fd) < 0)
	{
		printf("pipe error");
		goto EXIT;
	}
	if ((pid = fork()) < 0)
	{
		printf("fork error");
		goto EXIT;
	}
	else if (pid > 0)
	{			/* parent */
		printf("dad pid %d\n", getpid());
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
		//close(fd[1]); //if close here, the read2 in son_process will return 0; if no close, the read2 will block;
		sleep(1000);
	}
	else
	{			/* child */
		close(fd[1]);
		printf("son pid %d\n", getpid());
		n = read(fd[0], line, 256);
		if (n > 0)
			printf("read: %s\n", line);

		n = read(fd[0], line, 256);	//read2 
		printf("read %d\n", n);
	}
      EXIT:
	return 0;
}


//1, if the pipe is no data, the read will block;
//2, write will nerver block;
