#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#define FIFO_SERVER "/tmp/fifoserver"

int main(int argc, char **argv) 
{
	int fd;
	char w_buf[4096];
	int real_wnum;

	memset(w_buf, 0, 4096);
	if ((mkfifo(FIFO_SERVER, O_CREAT | O_EXCL) < 0) && (errno != EEXIST))
		printf("cannot create fifoserver\n");
	
        fd = open(FIFO_SERVER,O_WRONLY|O_NONBLOCK,0);
        //fd = open(FIFO_SERVER, O_WRONLY, 0);
	if (fd == -1)
            if (errno == ENXIO)
                    printf("open error; no reading process\n");
	
        real_wnum = write(fd, w_buf, 2048);
	if (real_wnum == -1)
	{
		if (errno == EAGAIN)
			printf("write to fifo error; try later\n");
	}
	else
		printf("real write num is %d\n", real_wnum);

	real_wnum = write(fd, w_buf, 5000);
	if (real_wnum == -1)
		if (errno == EAGAIN)
			printf("try later\n");
	printf("real write num is %d\n", real_wnum);
}


