#if 0 // zero copy for sendfile
// sev.c
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <assert.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <sys/sendfile.h>  
  
int main( int argc, char* argv[] )  
{  
    if( argc <= 4 )  
    {  
        //printf( "usage: %s ip_address port_number filename sendtype\n", basename( argv[0] ) );  
        printf( "usage: ip_address port_number filename sendtype\n");  
        return 0;  
    } 

    int longnum=0,sum=0;  
    static char buf[2048];  
    memset(buf,'\0',sizeof(buf));  
    const char* ip = argv[1];  
    int port = atoi( argv[2] );  
    const char* file_name = argv[3];
    int sendtype = atoi(argv[4]);
  
    int filefd = open( file_name, O_RDONLY ); 
    assert( filefd > 0 ); 
    struct stat stat_buf; 
    fstat( filefd, &stat_buf ); 
    printf("file size %d\n", stat_buf.st_size); 
    FILE *fp=fdopen(filefd,"r");  
          
    struct sockaddr_in address;  
    bzero( &address, sizeof( address ) );  
    address.sin_family = AF_INET;  
    inet_pton( AF_INET, ip, &address.sin_addr );  
    address.sin_port = htons( port );  
  
    int sock = socket( PF_INET, SOCK_STREAM, 0 );  
    assert( sock >= 0 );  
  
    int ret = bind( sock, ( struct sockaddr* )&address, sizeof( address ) );  
    assert( ret != -1 );  
  
    ret = listen( sock, 5 );  
    assert( ret != -1 );  
  
    struct sockaddr_in client;  
    socklen_t client_addrlength = sizeof( client );  
    int connfd = accept( sock, ( struct sockaddr* )&client, &client_addrlength );  
    if ( connfd < 0 )  
    {  
        printf( "errno is: %d\n", errno );  
    }  
    else  
    {  
        struct timeval tvstart, tvend;
        long timespend = 0;
        gettimeofday( &tvstart, NULL); 
        if (sendtype == 1)
        {
            while((fgets(buf,2048,fp))!=NULL)
            {  
                int num=send(connfd,buf,sizeof(buf),0);  
                sum+=num;  
                //memset(buf,'\0',sizeof(buf));  
            }
        }
        else
            sum = sendfile( connfd, filefd, NULL, stat_buf.st_size );  
        gettimeofday( &tvend, NULL);
        timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
        printf("sum:%ld, time spend %ld\n",sum, timespend);  
        close( connfd );  
    }  
  
    close( sock );  
    return 0;  
}  

//cli
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <assert.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <sys/sendfile.h>  
  
int main( int argc, char* argv[] )  
{  
    if( argc <= 3 )  
    {  
        printf( "usage: %s ip_address port_number filename\n", basename( argv[0] ) );  
        return 1;  
    }  
    static char buf[1024];  
    memset(buf,'\0',sizeof(buf));  
    const char* ip = argv[1];  
    int port = atoi( argv[2] );  
    const char* file_name = argv[3];  
  
    int filefd = open( file_name, O_WRONLY );  
    if(filefd<=0)  
        printf("open error:%s",strerror(errno));  
    assert( filefd > 0 );  
          
    FILE *fp=fdopen(filefd,"w");  
          
    struct sockaddr_in address;  
    socklen_t len=sizeof(address);  
    bzero( &address, sizeof( address ) );  
    address.sin_family = AF_INET;  
    inet_pton( AF_INET, ip, &address.sin_addr );  
    address.sin_port = htons( port );  
  
    int sock = socket( PF_INET, SOCK_STREAM, 0 );  
    assert( sock >= 0 );  
    int num;  
    int ret=connect(sock,(struct sockaddr*)&address,len);  
    
    if ( ret < 0 )  
    {  
        printf( "connect errno: %s\n", strerror(errno) );  
    }  
    else  
    {  
        while( (num=recv(sock,buf,sizeof(buf),0))>0 )
        {  
            fprintf(fp,"%s",buf);  
            memset(buf,'\0',sizeof(buf));  
        }  
          
        close( sock );  
    }  
  
    close( sock );  
    return 0;  
}  
#endif
#if 0
#include <stdio.h>
#include <arpa/inet.h>

#define OFFSET_MASK 0X1FFF
int main()
{
    unsigned short offset = 0;
    unsigned short n_offset = 0xb920;
    offset = (ntohs(n_offset) & OFFSET_MASK) << 3;
    printf("offset %d\n", offset);
    return 0;
}
#endif

#if 0  //calculate the tcp checksum
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PSEUDOHDR_TCP_LEN 12 
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

uchar *gen_pseudo_header(uchar *p_tcpcontext, uint len, uchar *p_ipheader)
{
    int i = 0;
    uchar *p_full_tcpcontext = (uchar *)malloc(sizeof(uchar)*(len+PSEUDOHDR_TCP_LEN));

    memcpy(p_full_tcpcontext+PSEUDOHDR_TCP_LEN, p_tcpcontext, len);
    // src ip(4) 
    // dst ip(4)
    // zero(1)+protocol(1)+tcp-total-len(2)
    memcpy(p_full_tcpcontext,  p_ipheader+12, 4);
    memcpy(p_full_tcpcontext+4,  p_ipheader+16, 4);
    *(p_full_tcpcontext+8) = 0;
    *(p_full_tcpcontext+9) = 6;
    *((ushort *)(p_full_tcpcontext+10)) = htons(len); //  should use ntohs() ?

    printf("pseudo_header: \n");
    for (i = 0; i < 12; i++)
    {
        printf("%02x ", *(p_full_tcpcontext+i) );
    }
    printf("\n");
    return p_full_tcpcontext ;
}

uint calculate_checksum(uchar *ptext, uint len)
{
    uint checksum = 0;
    ushort last_checksum = 0;
    uchar *p_new_context = NULL;
    if ((len % 2) != 0)
    {
        // regen the total context, let its len to be even.
        uchar *p_new_context = (uchar *)malloc(sizeof(uchar)*(len+1));
        memcpy(p_new_context, ptext, len);
        *(p_new_context+len-1) = 0;
        free(ptext);
        ptext = p_new_context;
        len += 1;

    }
    while (len > 0)
    {
        checksum += (htons(*((ushort *)ptext)));
        len -= 2;
        ptext+=2;
    }

    if ((checksum >> 16) != 0)
    {
        last_checksum = checksum & 0xffff;
        last_checksum += ((checksum>>16)&0xffff);
        checksum = last_checksum; 
    }
    checksum = ~checksum;
    free(p_new_context);
    return checksum;
}

int doChecksum(unsigned char *buff, int len)
{    
    int count = len;    
    unsigned int sum = 0;   
    unsigned int sum2 = 0;  
    unsigned short checksum = 0;    
    char *input = buff;            

    while(count > 1)    
    {       
        sum2 = *buff++ & 0xff;      
        sum2 = ((sum2<<8)&0xff00) + (*buff++ & 0xff);       
        sum += sum2;        
        count -= 2;    
    }    

    if(count > 0)    
    {        
        sum += (*buff<<8);    
    }    

    while(sum >> 16)    
    {        
        sum = (sum & 0xffff) + (sum >> 16)&0xffff;    
    }   

    checksum = ~sum;    
    return checksum;    
}

int main()
{
    uchar ipheader[] = {0x45,0x00,0x00,0xa2,0x1d,0x8d,0x40,0x00,0x80,0x06,0x00,0x00,0x0a,0x58,0x46,0x40,0x0a,0x58,0x46,0x1e};
    uchar tcpcontext[] = {0xe1,0x21,0x00,0x8b,0x43,0x72,0x30,0xa0,0xdd,0x18,0x5f,0x09,0x50,0x18,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xff,0x53,0x4d,0x42,0x25,0x00,0x00,0x00,0x00,0x18,0x07,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x28,0x04,0x00,0x08,0x40,0x01,0x0e,0x1a,0x00,0x00,0x00,0x08,0x00,0x16,0x11,0x00,0x00,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x1a,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x37,0x00,0x00,0x5c,0x00,0x50,0x00,0x49,0x00,0x50,0x00,0x45,0x00,0x5c,0x00,0x4c,0x00,0x41,0x00,0x4e,0x00,0x4d,0x00,0x41,0x00,0x4e,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x57,0x72,0x4c,0x65,0x68,0x44,0x4f,0x00,0x42,0x31,0x36,0x42,0x42,0x44,0x7a,0x00,0x01,0x00,0x16,0x11,0x00,0x00,0x00,0x80};

    uchar *p_full_tcpcontext;
    ushort checksum = 0;
    p_full_tcpcontext = gen_pseudo_header(tcpcontext, sizeof(tcpcontext), ipheader);
    checksum = calculate_checksum(p_full_tcpcontext, sizeof(tcpcontext)+12);
    printf("tcp checksum is 0x%x\n", checksum);
    checksum = doChecksum(p_full_tcpcontext, sizeof(tcpcontext)+12);
    printf("tcp checksum is 0x%x\n", checksum);
    checksum = calculate_checksum(ipheader, 20);
    printf("ip checksum is 0x%x\n", checksum);
    checksum = doChecksum(ipheader, 20);
    printf("ip checksum is 0x%x\n", checksum);
    return 0;
}

#endif


#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <signal.h>

static int do_abort = 0;

void handle_signal(int signo)
{
    if (signo == SIGHUP)
    {
        printf("child recv SIGHUP..\n");
        do_abort = 1;
    }
}

int main(void)
{
    pid_t pid;
    pid = fork();
    char *p = NULL;

    if (pid == 0) // child
    {
        signal(SIGHUP, handle_signal);
        //prctl(PR_SET_PDEATHSIG, SIGHUP);
        while(!do_abort) {
            sleep(1);
            printf("in child...\n");
        }
        printf("child exit...\n");
        return 0;
    }
    else // parent
    {
        int times = 5;
        while(times-- > 0)
        {
            sleep(1);
            if (times == 3)
            {
                printf("memcpy ...\n");
                memcpy(p, "Hello", 5);
            }
            printf("in parent.\n");
        }

        printf("parent exit..\n");
        return 0;
    }

    return 0;
}

#endif

#if 0 // ~0U
#include <stdio.h>
int main(void)
{
    printf("\n~0U = %x",~0U);
    printf("\n~0U = %u",(unsigned short)~0U);
    printf("\n~0 = %x",~0);
    printf("\n~0U>>1 = %d",~0U>>1);
    printf("\n~0>>1 = %d\n",~0>>1);

    unsigned short a = 0;

    if (a != (unsigned short)~0U)
        printf("no 0\n");
    else
        printf("0\n");
    return 0;
}

#endif

#if 0 // Reading from an invaild address will make the process crash.
#include <string.h>
int main()
{
    int *p = NULL;
    int array[6];
    p-=0xc;
    memcpy(array, p, 6);
    return 0;
}
#endif
#if 0
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int fd, i;
	char path[] = "./file.txt";
	extern int errno;

	fd = open(path, O_WRONLY | O_CREAT);
	if (fd != -1)
	{
		printf("open file %s .\n", path);
		printf("please input a number to lock the file.\n");
		scanf("%d", &i);
		printf("try lock file:%s...\n", path);

		if (flock(fd, LOCK_EX) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}
		printf("please input a number to unlock the file.\n");
		scanf("%d", &i);
		if (flock(fd, LOCK_UN) == 0)
		{
			printf("the file was unlocked.\n");
		}
		else
		{
			printf("the file was not unlocked.\n");
		}
		close(fd);
	}
	else
	{
		printf("cannot open file %s/n", path);
		printf("errno:%d\n", errno);
		printf("errMsg:%s\n", strerror(errno));
	}

	return 0;
}
#endif

#if 0
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int test_close = 1;
	int fd, i;
	char path[] = "/weiboad/work/py_work/file.txt";
	extern int errno;

	fd = open(path, O_WRONLY | O_CREAT);
	if (fd != -1)
	{
		printf("open file %s .\n", path);
		printf("please input a number to lock the file.\n");
		scanf("%d", &i);
		printf("try lock file:%s...\n", path);

		if (flock(fd, LOCK_EX) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}

		int fd1 = dup(fd);

		printf("fd:%d has dup, new fd:%d\n", fd, fd1);
		sleep(5);
		if (!test_close)
		{
			if (flock(fd, LOCK_UN) == 0)
			{
				printf("unlock success\n");
			}
			else
			{
				printf("unlock fail\n");
			}
		}
		else
		{
			close(fd);
			printf("fd:%d has closed\n", fd);
		}

		sleep(5);
	}
	else
	{
		printf("cannot open file %s/n", path);
		printf("errno:%d\n", errno);
		printf("errMsg:%s\n", strerror(errno));
	}

	printf("end\n");
	return 0;
}
#endif

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int fd = 0;
int main()
{
	int i;
	char path[] = "./file.txt";
	extern int errno;

	fd = open(path, O_WRONLY | O_CREAT);
	if (fd != -1)
	{
		printf("open file %s. fd:%d.\n", path, fd);
		printf("please input a number to lock the file.\n");
		scanf("%d", &i);
		printf("try lock file:%s...\n", path);

		if (flock(fd, LOCK_EX) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}

		int pid;

		if ((pid = fork()) < 0)
		{
			printf("fork failed\n");
			exit(0);
		}
		else if (pid == 0)
		{		// child
			sleep(5);

			//if(flock(fd,LOCK_SH)==0) {
			//  printf("child add ex success\n");
			//} else { 
			//  printf("child add ex failed\n");
			//}

			if (flock(fd, LOCK_UN) == 0)
			{
				printf("child unlock success.\n");
			}
			else
			{
				printf("child unlock fail.\n");
			}
			sleep(5);
		}
		else
		{		// parent
			int pid2 = wait(NULL);

			printf("end\n");
		}

	}
	else
	{
		printf("cannot open file %s/n", path);
		printf("errno:%d\n", errno);
		printf("errMsg:%s\n", strerror(errno));
	}

	return 0;
}
#endif

#if 0				//flock
#include <stdio.h>
#include <pthread.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


void readfile_thread(char *filepath)
{
	int fd = open(filepath, O_RDONLY);

	if (fd == -1)
	{
		printf("read file error, %s.\n", strerror(errno));
		return;
	}
	flock(fd, LOCK_EX);
	printf("lock the file for read.\n");
	sleep(100);
	printf("sub process end.\n");
	close(fd);
	return;
}

int main()
{
	char filepath[] = "/home/wt/code/git/tmp.txt";
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, readfile_thread, filepath);

	sleep(2);
	int fd = open(filepath, O_RDONLY);

	if (fd == -1)
	{
		printf("read file error, %s.\n", strerror(errno));
		return;
	}
	flock(fd, LOCK_EX);
	printf("main process end.\n");
	return 0;
}


#endif


#if 0				// system() issue
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

typedef void (*sighandler_t) (int);
int pox_system(const char *cmd_line)
{
	int ret = 0;
	sighandler_t old_handler;

	//old_handler = signal(SIGCHLD, SIG_DFL);
	old_handler = signal(SIGCHLD, SIG_IGN);
	ret = system(cmd_line);
	printf("system return %d\n", ret);
	signal(SIGCHLD, old_handler);

	return ret;
}


int main()
{
	int err = 0;

	err = pox_system("who");
	if (err != 0)
		printf("execute who failed, %s.\n", strerror(errno));
	return 0;
}

#endif


#if 0				// endian
#include <stdio.h>
#include <arpa/inet.h>

int check1()
{
	int i = 1;		//1在内存中的表示： 0x00000001  
	char *pi = (char *)&i;	//将int型的地址强制转换为char型  

	return *pi == 0;	//如果读取到的第一个字节为1，则为小端法，为0，则为大端法  
}


int main()
{
	printf("int size %d\n", sizeof(int));
	if (check1())
		printf("Your system is big endian.\n");
	else
		printf("Your system is little endian.\n");

	int a = 0x12345678;

	// little endian
	printf("%x %x %x %x\n", *((char *)(&a)), *((char *)(&a) + 1), *((char *)(&a) + 2), *((char *)(&a) + 3));
	a = htonl(a);
	// big endian
	printf("%x %x %x %x\n", *((char *)(&a)), *((char *)(&a) + 1), *((char *)(&a) + 2), *((char *)(&a) + 3));
	return 0;
}

#endif


#if 0				// get/set system time and mtrace

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct timespec res;
	int ret = 0;

	setenv("MALLOC_TRACE", "memory-leak.log", "1");
	mtrace();
	clock_gettime(CLOCK_MONOTONIC, &res);
	printf("time %u %u\n", res.tv_sec, res.tv_nsec);

	char *p = malloc(2000);

	char *p2 = p;

	if (ret)
		free(p2);

	return 0;
}



#endif


#if 0
#include <stdio.h>
#include <signal.h>

//char log[1024];
#if 0
void printReg()
{
	unsigned int spReg, lrReg, pcReg;

	__asm
	{
	MOV spReg, __current_sp() MOV pcReg, __current_pc() MOV lrReg, __return_address()}
	printf("SP = 0x%X\n", spReg);
	printf("PC = 0x%X\n", pcReg);
	printf("LR = 0x%X\n", lrReg);
}
#endif

void segment_handle(int signum, siginfo_t * siginfo, void *context)
{
	char *signame;
	ucontext_t *ucontext;
	struct sigcontext *usigcontext;

	switch (signum)
	{
	case SIGSEGV:
		signame = "SIGSEGV";
		break;
	case SIGFPE:
		signame = "SIGFPE";
		break;
	default:
		break;
	}

	ucontext = (ucontext_t *) context;
	usigcontext = (struct sigcontext *)&(ucontext->uc_mcontext);
	if ((signum == SIGSEGV) || (signum == SIGFPE))
	{
		printf
		    ("sig %s: segment fault addr=%016lx, cs=%016lx, pc=%016lx, lr/r14=%016lx, r13=%016lx\n",
		     signame, (long)(siginfo->si_addr), usigcontext->cs,
		     usigcontext->rip, usigcontext->r14, usigcontext->r13);
	}
	//printReg();

	exit(0);		// why can't use return: exit() end of process;
}

int main()
{
	//int *p = NULL;
	char a[3] = { 0 };
	int ret = 0;
	struct sigaction sigact;

	sigact.sa_sigaction = segment_handle;
	sigact.sa_flags = SA_SIGINFO;
	ret = sigaction(SIGSEGV, &sigact, NULL);
	ret = sigaction(SIGFPE, &sigact, NULL);
	//a[8000] = 1; // why 8000 seg, 7000 works well
	//printf("log %d\n", log);
	ret = a[1] / 0;
	return 0;
}

#endif
#if 0				// buff for file io
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
	int i;
	int ret = 0;
	FILE *fp;
	char msg1[] = "hello,world\n";
	char msg2[] = "hello\nworld";
	char msg[128] = { 0 };
	char buf[128];

	if ((fp = fopen("no_buf1.txt", "w")) == NULL)
	{
		perror("file open fail");
		exit(-1);
	}
	setbuf(fp, NULL);
	fwrite(msg1, 7, 1, fp);
	//fseek(fp, 0, SEEK_SET);
	//ret = fgets(msg, 128, fp);
	//printf("str %s\n", strerror(errno));
	//printf("get %d %s\n", ret, msg);
	printf("test setbuf(no buf)!check no_buf1.txt\n");	//查看 buf 情况
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf（如果有 buf 的话）

	/* 打开（或者创建）一个文件，然后使用 setvbuf 设置为 nobuf 情况，并检查关闭前流的情况 */
	if ((fp = fopen("no_buf2.txt", "w")) == NULL)
	{
		perror("file open failure!");
		exit(-1);
	}
	setvbuf(fp, NULL, _IONBF, 0);	//设置为无 buf
	fwrite(msg1, 7, 1, fp);
	printf("test setbuf(no buf)!check no_buf2.txt\n");	//查看 buf 情况
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf( 如果有 buf 的话)

	if ((fp = fopen("1_buf.txt", "w")) == NULL)
	{
		perror("fail open file");
		exit(-1);
	}
	setvbuf(fp, buf, _IOLBF, sizeof(buf));	//设置为行 buf
	fwrite(msg2, sizeof(msg2), 1, fp);	//写内容
	printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");	//查看 buf 情况
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf

	//打开（或者创建）一个文件，然后使用 setvbuf 设置为全 buf情况，并检查关闭前流的情况
	if ((fp = fopen("f_buf.txt", "w")) == NULL)
	{
		perror("file open failure!");
		exit(-1);
	}
	setvbuf(fp, buf, _IOFBF, sizeof(buf));
	for (i = 0; i < 2; i++)
	{
		fputs(msg1, fp);
	}
	printf("test setbuf(full buf)! check f_buf.txt\n");
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf
	return 0;
}
#endif

#if 0
/* 
 * use fopen in w+/r+: 
 * r+     Open for reading and writing.  The stream is positioned at the beginning of the file.
 * w+     Open for reading and writing.  The  file is created if it does not exist, otherwise it is truncated.  
 *        The stream is positioned at the  beginning of the file.
 */
#include <stdio.h>
int main()
{
	FILE *fp = NULL;
	FILE *fp2 = NULL;
	char cmd[128] = { 0 };


	fp = fopen("txt", "w+");
	if (fp != NULL)
	{
		while (fgets(cmd, sizeof(cmd), fp) != NULL)
		{
			printf("%s\n", cmd);
		}
		fclose(fp);
	}

	fp2 = fopen("txt1", "r+");
	if (fp2 != NULL)
	{
		while (fgets(cmd, sizeof(cmd), fp2) != NULL)
		{
			printf("%s\n", cmd);
		}
		fclose(fp2);
	}
	else
	{
		printf("file doesn't exist\n");
	}

	return 0;
}

#endif

#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

void print_info(char *var_name, char *var_ptr, size_t size_in_kb)
{
	printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n", var_name, size_in_kb, var_ptr, sbrk(0));
}

int main(int argc, char **argv)
{
	char *heap_var1, *heap_var2, *heap_var3;
	char *mmap_var1, *mmap_var2, *mmap_var3;
	char *maybe_mmap_var;

	printf("Orginal heap top is 0x%lx\n", sbrk(0));
	heap_var1 = malloc(32 * 1024);
	print_info("heap_var1", heap_var1, 32);
	heap_var2 = malloc(64 * 1024);
	print_info("heap_var2", heap_var2, 64);
	heap_var3 = malloc(127 * 1024);
	print_info("heap_var3", heap_var3, 127);
	printf("\n");
	maybe_mmap_var = malloc(128 * 1024);
	print_info("maybe_mmap_var", maybe_mmap_var, 128);
	//mmap
	mmap_var1 = malloc(128 * 1024);
	print_info("mmap_var1", mmap_var1, 128);
	// set M_MMAP_THRESHOLD to 64k
	mallopt(M_MMAP_THRESHOLD, 64 * 1024);
	printf("set M_MMAP_THRESHOLD to 64k\n");
	mmap_var2 = malloc(64 * 1024);
	print_info("mmap_var2", mmap_var2, 64);
	mmap_var3 = malloc(127 * 1024);
	print_info("mmap_var3", mmap_var3, 127);
	return 1;
}
#endif


#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>
char ps_cmd[1024];
void print_info(char *var_name, char *var_ptr, size_t size_in_kb)
{
	printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n", var_name, size_in_kb, var_ptr, sbrk(0));
	system(ps_cmd);
}

int main(int argc, char **argv)
{
	char *non_set_var, *set_1k_var, *set_5k_var, *set_7k_var;
	pid_t pid;

	pid = getpid();
	sprintf(ps_cmd, "ps aux | grep %lu | grep -v grep|awk '{print \"VSZ: \"$5\" RSS: \" $6}'", pid);
	non_set_var = malloc(32 * 1024);
	print_info("non_set_var", non_set_var, 32);
	set_1k_var = malloc(64 * 1024);
	memset(set_1k_var, 0, 1024);
	print_info("set_1k_var", set_1k_var, 64);
	set_5k_var = malloc(127 * 1024);
	memset(set_5k_var, 0, 5 * 1024);
	print_info("set_5k_var", set_5k_var, 127);
	set_7k_var = malloc(64 * 1024);
	memset(set_1k_var, 0, 7 * 1024);
	print_info("set_7k_var", set_7k_var, 64);
	return 1;
}
#endif

#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

size_t heap_malloc_total, heap_free_total, mmap_total, mmap_count;

void print_info()
{
	struct mallinfo mi = mallinfo();

	printf("count by itself:\n");
	printf
	    ("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
	     heap_malloc_total * 1024, heap_free_total * 1024,
	     heap_malloc_total * 1024 - heap_free_total * 1024, mmap_total * 1024, mmap_count);
	printf("count by mallinfo:\n");
	printf
	    ("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu, %lu, %lu, %lu\n",
	     mi.arena, mi.fordblks, mi.uordblks, mi.hblkhd, mi.hblks, mi.fsmblks, mi.smblks, mi.ordblks);
	printf("from malloc_stats:\n");
	malloc_stats();
}

#define ARRAY_SIZE 200
int main(int argc, char **argv)
{
	char **ptr_arr[ARRAY_SIZE];
	int i;

	for (i = 0; i < ARRAY_SIZE; i++)
	{
		ptr_arr[i] = malloc(i * 1024);
		if (i < 128)
			heap_malloc_total += i;
		else
		{
			mmap_total += i;
			mmap_count++;
		}

	}
	print_info();
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		if (i % 2 == 0)
			continue;
		free(ptr_arr[i]);
		if (i < 128)
			heap_free_total += i;
		else
		{
			mmap_total -= i;
			mmap_count--;
		}
	}
	printf("\nafter free\n");
	print_info();
	return 1;
}
#endif

#if 0
#include <stdio.h>
#include <string.h>

int main()
{
	//unsigned char aa[] = "hello:world:fuck you then";
	unsigned char aa[] = "1.1.1.1;;;;2.2.2.2";

	//unsigned char *aa = "hello:::world:fuck you then"; 
	unsigned char *pc1 = NULL;
	unsigned char *pc2 = NULL;

	pc1 = strtok_r(aa, ";", &pc2);
	while (pc1)
	{
		printf("aa(len %d) %s, pc1 %s, pc2 %s\n", strlen(aa), aa, pc1, pc2);
		//printf("addr: aa %p, pc1 %p, pc2 %p\n", aa, pc1, pc2);
		pc1 = strtok_r(NULL, ";", &pc2);
	}
	return 0;
}
#endif

// byte align
#if 0
#include <stdio.h>

struct A
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
};

struct B
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
} __attribute__ ((aligned));

struct C
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
} __attribute__ ((aligned(1)));

struct D
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
} __attribute__ ((aligned(4)));

struct E
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
} __attribute__ ((aligned(8)));

struct F
{
	char a;
	int b;
	unsigned short c;
	long d;
	unsigned long long e;
	char f;
} __attribute__ ((packed));

int main(int argc, char **argv)
{
	printf("long %d, longlong %d, short %d, int %d\n", sizeof(long),
	       sizeof(unsigned long long), sizeof(short), sizeof(int));
	printf("A = %d, B = %d, C = %d, D = %d, E = %d, F = %d\n",
	       sizeof(struct A), sizeof(struct B), sizeof(struct C),
	       sizeof(struct D), sizeof(struct E), sizeof(struct F));
	printf("%x %x %x %x %x %x\n", &(((struct A *)0)->a),
	       &(((struct A *)0)->b), &(((struct A *)0)->c),
	       &(((struct A *)0)->d), &(((struct A *)0)->e), &(((struct A *)0)->f));
	printf("%x %x %x %x %x %x\n", &(((struct B *)0)->a),
	       &(((struct B *)0)->b), &(((struct B *)0)->c),
	       &(((struct B *)0)->d), &(((struct B *)0)->e), &(((struct B *)0)->f));
	return 0;
}

#endif

#if 0				// variables in the memory layout
/*
 *   kernel space     
 *   
 *   statck space
 *   ...                
 *   heap space        
 *   ...
 *   bss             => uninitialized global and static variables  
 *   data            => initialized global and static variables
 *   constant
 *   text
 * */
#include <stdio.h>
int a;
int b = 10;
void func()
{
	return;
}

int main()
{
	char *p = "hello wolrd";
	static int c;
	static int d = 10;

	printf("%p\n", p);	//constant area
	printf("%p\n", &p);
	printf("%p\n", &a);
	printf("%p\n", &b);
	printf("%p\n", &c);
	printf("%p\n", &d);
	printf("%p\n", func);
	return 0;
}

#endif
#if 0
#include < stdio.h>
#include < stdlib.h>
int main()
{
	int i;
	FILE *fp;
	char msg1[] = "hello,world\n";
	char msg2[] = "hello/nworld\n";
	char buf[128];

	/*打开（或者创建）一个文件，然后使用setbuf设置为nobuf情况，并检查关闭前流的情况 */
	if ((fp = fopen("no_buf1.txt", "w")) == NULL)
	{
		perror("file open fail");
		exit(-1);
	}
	setbuf(fp, NULL);
	fwrite(msg1, 7, 1, fp);
	printf("test setbuf(no buf)!check no_buf1.txt\n");	//查看 buf 情况 
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf（如果有 buf 的话）
	/*打开（或者创建）一个文件，然后使用 setvbuf 设置为 nobuf 情况，并检查关闭前流的情况 */
	if ((fp = fopen("no_buf2.txt", "w")) == NULL)
	{
		perror("file open failure!");
		exit(-1);
	}
	setvbuf(fp, NULL, _IONBF, 0);	//设置为无 buf 
	fwrite(msg1, 7, 1, fp);
	printf("test setbuf(no buf)!check no_buf1.txt\n");	//查看 buf 情况 
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf ( 如果有 buf 的话)
	if ((fp = fopen("1_buf.txt", "w")) == NULL)
	{
		perror("fail open file");
		exit(-1);
	}
	setvbuf(fp, buf, _IOLBF, sizeof(buf));	//设置为行 buf
	fwrite(msg2, sizeof(msg2), 1, fp);	//写内容
	printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");	//查看 buf 情况 
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf 
	//打开（或者创建）一个文件，然后使用 setvbuf 设置为全 buf情况，并检查关闭前流的情况 
	if ((fp = fopen("f_buf.txt", "w")) == NULL)
	{
		perror("file open failure!");
		exit(-1);
	}
	setvbuf(fp, buf, _IOFBF, sizeof(buf));
	for (i = 0; i < 2; i++)
	{
		fputs(msg1, fp);
	}
	printf("test setbuf(full buf)! check f_buf.txt\n");
	printf("press enter to continue!\n");
	getchar();
	fclose(fp);		//关闭流，因此将回写 buf 
	return 0;
}

#endif

#if 0				// how long is the printf buff
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int c = atoi(argv[1]);	// 1024 no output; 1025 output

	while (c-- > 0)
		printf("a");
	while (1) ;
	return 0;
}
#endif
#if 0
#include <stdio.h>
int function(int a, int b, int c)
{
	char buffer[14];
	int sum;
	int *ret;

	ret = (int *)(buffer + 26);
	(*ret) += 8;
	sum = a + b + c;
	return sum;
}

int main()
{
	int x;

	x = 0;
	function(1, 2, 3);
	x = 1;

	printf("%d\n", x);
	return 0;
}
#endif

#if 0			//vfork
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int glob = 6;
int main(void)
{
	int var;
	pid_t pid;

	var = 88;
	printf("before vfork\n");
	if ((pid = vfork()) < 0)
	{
		printf("vfork error");
		exit(-1);
	}
	else if (pid == 0)
	{
		glob++;
		var++;
		//return 0;
        //sleep(10);
		//exit(0);
	}
    else
    {
        glob--;
        var--;
    }
    //sleep(10);
	printf("pid=%d, glob=%d, var=%d\n", getpid(), glob, var);
	return 0;
}
#endif

#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main()
{
	struct stat buf;
	int fd = 0;

	stat("test.txt", &buf);
	printf("1.link=%d\n", buf.st_nlink);	//未打开文件之前测试链接数
	fd = open("test.txt", O_CREAT);	//打开已存在文件test.txt
	stat("test.txt", &buf);
	printf("2.link=%d\n", buf.st_nlink);	//测试链接数

	close(fd);		//关闭文件test.txt
	stat("test.txt", &buf);
	printf("3.link=%d\n", buf.st_nlink);	//测试链接数

	link("test.txt", "test2.txt");	//创建硬链接test2.txt
	stat("test.txt", &buf);
	printf("4.link=%d\n", buf.st_nlink);	//测试链接数

	unlink("test2.txt");	//删除test2.txt
	stat("test.txt", &buf);
	printf("5.link=%d\n", buf.st_nlink);	//测试链接数

	fd = open("test.txt", O_RDONLY);	//打开已存在文件test.txt
	stat("test.txt", &buf);
	printf("6.link=%d\n", buf.st_nlink);	//测试链接数

	unlink("test.txt");	//删除test.txt
	fstat(fd, &buf);	//use fstat not stat
	printf("7.link=%d\n", buf.st_nlink);	//测试链接数

	close(fd);
	fstat(fd, &buf);
	printf("8.link=%d\n", buf.st_nlink);	//测试链接数

	return 0;
}
#endif

#if 0				// open,link,close,unlink
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fd;
	char buf[32];
	struct stat buff;
	struct stat buff2;

	if ((fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) < 0)
	{
		printf("create file error!\n");
	}
	if (write(fd, "temp", 5) < 0)
	{
		printf("write wrror!\n");
	}			//后面unlink到0后，目录里没有文件了，但是fd还可以访问其中内容！，因为本进程还没有close，如果close之后就啥都没鸟~

	stat("temp.txt", &buff);
	printf("temp.link=%d\n", buff.st_nlink);
	link("temp.txt", "test.txt");
	stat("test.txt", &buff);
	printf("after link the tem.link =%d\n", buff.st_nlink);
	if (unlink("test.txt") < 0)
	{
		printf("unlink error !\n");
	}
	stat("temp.txt", &buff);
	printf("after unlink tem.link=%d\n", buff.st_nlink);
	if (unlink("temp.txt") < 0)
	{
		printf("unlink error !\n");
	}

	//此处我们改用fstat函数而非stat，因为unlilnk已经删除文件名，所以不可以通过文件名访问
	//但是fd仍然是打开着的，文件内容还没有被真正删除，依旧可以使用fd获得文件信息
	fstat(fd, &buff);
	printf("after unlink tem.link=%u\n", buff.st_nlink);

	if ((lseek(fd, 0, SEEK_SET)) == -1)
	{
		printf("lseek error!\n");
	}

	//why can read after the link number is 0
	if ((read(fd, buf, 5)) < 0)
	{
		printf("read error!\n");
	}
	printf("%s,then ,close fd ,reopen \n", buf);
	close(fd);

	if ((lseek(fd, 0, SEEK_SET)) == -1)
	{
		printf("lseek error!\n");
	}

	memset(buf, 0, sizeof(buf));
	if ((read(fd, buf, 5)) < 0)
	{
		printf("read error!\n");
	}
	printf("%s after reopen \n", buf);
	return 0;
}

#endif

#if 0				// 3.13
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main(void)
{
	vector < int >num, sum;
	int tmp = 0;
	int count = 0;
	int index = 0;
	int i = 0;

	cout << "please input numbers: " << endl;
	while (cin >> tmp)
	{
		num.push_back(tmp);
		if ((count >= 2) && ((count % 2) == 0))
		{
			sum[index++] = num[count - 2] + num[count - 1];
		}
		cin >> num[count++];
	}
	--count;

	if ((count % 2) != 0)
	{
		cout << "Total number is " << count << " the last one is " << num[count - 1] << endl;
	}

	for (i = 0; i < count; i++)
	{
		cout << num[i] << " ";
	}

	cout << endl;

	cout << "use iterator to walk vector<int> type: " << endl;
	for (vector < int >::iterator iter = num.begin(); iter != num.end(); iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;

	for (i = 0; i < index; i++)
	{
		cout << sum[i] << "   ";
	}
	cout << endl;

	return 0;
}
#endif
