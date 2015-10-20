#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>
#include <string.h>

int main(void)
{
	int handle, ret;

	char msg[] = "This is a test";

	char ch;

	/* create a file */
	handle = open("TEST.$$$", O_CREAT | O_RDWR, 0);
	/* write some data to the file */
	write(handle, msg, (int)strlen(msg));
	/* seek to the begining of the file */
	lseek(handle, 2, SEEK_SET);
	/* reads chars from the file until we hit EOF */
	while (ret = read(handle, &ch, 1))
	{
		printf("%c", ch);
	}
	printf("\n");
	close(handle);
	return 0;
}
