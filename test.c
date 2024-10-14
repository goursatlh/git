#test for git
# test
# test for branch
#if 1 // COW
#define _XOPEN_SOURCE 700
#include <fcntl.h> /* open */
#include <stdint.h> /* uint64_t  */
#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <unistd.h> /* pread, sysconf */
#include <sys/types.h>

/*************************************** virt_to_phys apis begin *******************************************/
typedef struct {
    uint64_t pfn : 55;
    unsigned int soft_dirty : 1;
    unsigned int file_page : 1;
    unsigned int swapped : 1;
    unsigned int present : 1;
} PagemapEntry;

/* Parse the pagemap entry for the given virtual address.
 *
 * @param[out] entry      the parsed entry
 * @param[in]  pagemap_fd file descriptor to an open /proc/pid/pagemap file
 * @param[in]  vaddr      virtual address to get entry for
 * @return 0 for success, 1 for failure
 */
int pagemap_get_entry(PagemapEntry *entry, int pagemap_fd, uintptr_t vaddr)
{
    size_t nread;
    ssize_t ret;
    uint64_t data;
    uintptr_t vpn;

    vpn = vaddr / sysconf(_SC_PAGE_SIZE);
    nread = 0;
    while (nread < sizeof(data)) {
        ret = pread(pagemap_fd, ((uint8_t*)&data) + nread, sizeof(data) - nread,
                vpn * sizeof(data) + nread);
        nread += ret; 
        if (ret <= 0) {
            return 1;
        }
    }
    entry->pfn = data & (((uint64_t)1 << 55) - 1);
    entry->soft_dirty = (data >> 55) & 1;
    entry->file_page = (data >> 61) & 1;
    entry->swapped = (data >> 62) & 1;
    entry->present = (data >> 63) & 1;
    return 0;
}

/* Convert the given virtual address to physical using /proc/PID/pagemap.
 *
 * @param[out] paddr physical address
 * @param[in]  pid   process to convert for
 * @param[in] vaddr virtual address to get entry for
 * @return 0 for success, 1 for failure
 */
int virt_to_phys_user(uintptr_t *paddr, pid_t pid, uintptr_t vaddr)
{
    char pagemap_file[BUFSIZ];
    int pagemap_fd;

    snprintf(pagemap_file, sizeof(pagemap_file), "/proc/%ju/pagemap", (uintmax_t)pid);
    pagemap_fd = open(pagemap_file, O_RDONLY);
    if (pagemap_fd < 0) {
        return 1;
    }
    PagemapEntry entry;
    if (pagemap_get_entry(&entry, pagemap_fd, vaddr)) {
        return 1;
    }
    close(pagemap_fd);
    *paddr = (entry.pfn * sysconf(_SC_PAGE_SIZE)) + (vaddr % sysconf(_SC_PAGE_SIZE));
    return 0;
}
/**************************************** virt_to_phys apis end ******************************************************/

/* test */
int data = 10;
 
int child_process()
{
    uintptr_t paddr = 0;
    virt_to_phys_user(&paddr, getpid(), (uintptr_t)(&data));
	//printf("Child process %d, data %d addr %p\n", getpid(), data, virt_to_phys(&data));
	printf("Child  process %d, data %d vir_addr %p, phy_addr %lx\n", getpid(), data, &data, paddr);
	data = 20;
    paddr = 0;
    virt_to_phys_user(&paddr, getpid(), (uintptr_t)(&data));
	printf("Child  process %d, data %d vir addr %p, phy_addr %lx\n", getpid(), data, &data, paddr);
    sleep(6000);
	_exit(0);
}
 
int main(int argc, char *argv[])
{
	int pid;
	pid = fork();
 
	if (pid == 0) {
		child_process();
	} else {
        uintptr_t paddr = 0;
		sleep(1);
        virt_to_phys_user(&paddr, getpid(), (uintptr_t)(&data));
	    printf("Parent process %d, data %d vir addr %p, phy_addr %lx\n", getpid(), data, &data, paddr);
		sleep(6000);
		exit(0);
	}
}
#endif

#if 0 // how to display the time in format
#include <stdio.h>            
#include <sys/time.h>         
#include <time.h>             

int main()
{
    char str[128]; 
    struct timeval tv; 
    struct tm *tmp;
    time_t now;

    // method 1
    gettimeofday(&tv, NULL); 
    tmp = localtime(&tv.tv_sec);   
    strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", tmp); 
    printf(">>>>>>> %s.%03ld\n", str, tv.tv_usec / 1000);

    // method 2
    time(&now);
    tmp = localtime(&now);
    strftime(str, sizeof(str), "@<%Y%m%d %H:%M:%S>", tmp);
    printf(">>>>>>> %s\n", str);  

    return 0;
}
#endif


#if 0 // duplicate name between user apis and glibc 
//#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>

int system(__const char *__command) __wur
{
    printf("execute %s\n", __command);
    return 0;
}

int main()
{
    system("who");
    return 0;
}
#endif


#if 0 // inlie assembly to c 2
#include <stdio.h>

#if 0
int call_teq(int x)
{
        unsigned long tmp;

        __asm__ __volatile__(
"       ldrex   %0, [%1]\n"
"       teq     %0, #0\n"
"       strexeq %0, %2, [%1]"
        : "=&r" (tmp)
        : "r" (x), "r" (1)
        : "cc");

        printf("tmp %d\n", tmp);

        return tmp;
}
#endif

int call(int x,int y)
{
        int sum = 0;
        asm("movl %[val1],%%ebx\n\t"
            "movl %[val2],%%ecx\n\t"
            "addl %%ebx,%%ecx\n\t"
            "movl %%ecx,%[SUM]"
            :[SUM] "=r" (sum)      //这个是output-list
            :[val1] "r" (x),[val2] "r" (y)    //这个是input-list
            :"%ebx","%ecx"          //这个是overwriter list
        );
        return sum;
}

int callnum(int x,int y)
{
        int sum = 0;
        asm("@ callnum \n"
            "movl %1,%%ebx\n\t"
            "movl %2,%%ecx\n\t"
            "addl %%ebx,%%ecx\n\t"
            "movl %%ecx,%0"
            :"=r" (sum)      //这个是output-list if no name, start number 0,1,2 from outputlist to inputlist
            :"r" (x), "r" (y)    //这个是input-list
            :"%ebx","%ecx"          //这个是overwriter list
        );
        return sum;
}

int main()
{
        int a=15;
        int b=20;
        //printf("%d\n",call(a,b));
        printf("%d\n",callnum(a,b));
        return 0;
}
#endif

#if 0 // inline assembly to c
#include <stdio.h>

int main()
{
    int a = 10, b = 0;

    __asm__  __volatile__ (
    "movl %1, %%eax; \n\t"
    "movl %%eax, %0;"

    :"=r"(b)   // output
    :"r"(a)    // input
    :"%eax");

	printf("Result: %d, %d \n", a, b);
	return 0;
}

#endif



#if 0 // select deadloop
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    char buf[256];
    fd_set rset;
    int ret = 0;
    struct timeval tv;

    tv.tv_sec = 100;
    tv.tv_usec = 0;
    FD_ZERO(&rset);

    while (1)
    {
        FD_SET(STDIN_FILENO, &rset);
        printf("select tv %lu %lu\n", tv.tv_sec, tv.tv_usec);   
        ret = select(STDIN_FILENO+1, &rset, NULL, NULL, &tv);
        printf("select ret %d tv %lu %lu\n", ret, tv.tv_sec, tv.tv_usec);   
        if (ret >= 0)
        {
            if (FD_ISSET(STDIN_FILENO, &rset))
            {
                ret = read(STDIN_FILENO, buf, 256);
                printf("hello world! %d tv %lu %lu\n", ret, tv.tv_sec, tv.tv_usec);   
            }
        }
    }
    return 0;
}
#endif

#if 0 // flock vs execl
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include <unistd.h>
#include <pthread.h>

void *thread_function(void *arg)
{
        FILE * in = NULL;
        char *path = "/home/wt/code/git/file.txt";

        printf("thread enter. pid %d\n", getpid());
        in = fopen(path, "r");
        if (in == NULL)
        {
                return NULL;
        }

	if (flock(fileno(in), LOCK_SH) == 0)
	{       
		printf("Acquire the flock. pid %d\n", getpid());
	}
	else    
	{       
		printf("the file was not locked.\n");
	}

        sleep(10000000);
        fclose(in);
        printf("Free the flock. pid %d\n", getpid());
        return NULL;
}

int main()
{
        pthread_t mythread;
        printf("main enter. pid %d\n", getpid());
        if (pthread_create(&mythread, NULL, thread_function, NULL))
        {
                printf("error creating thread\n");
        }
        sleep(5);
        execl("/home/wt/code/git/hello", "/home/wt/code/git/hello", NULL);
        return 0;
}
#endif


#if 0
#include <stdio.h>
int main()
{
    int a[10];
    printf("%x\n", a);
    printf("%x\n", &a);
    printf("%x\n", a+1); // pointer to the next element in the array
    printf("%x\n", &a+1);// pointer to the position next to the whole array
    return 0;
}
#endif


//fopen fopen w w+ r r+
#if 0
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_PATH "data"
int main()
{
    FILE *fp = NULL;
    char buff[512] = {0};
    char buff1[32] = "hello world\n";
    struct stat fs;
    int flen = 0;
    int pos = 0;
    stat( FILE_PATH, &fs);
    printf("file %s size %d before fopen\n", FILE_PATH, fs.st_size);
    fp = fopen(FILE_PATH, "w+");
    stat( FILE_PATH, &fs);
    printf("file %s size %d after fopen\n", FILE_PATH, fs.st_size);
    //setvbuf ( fp , sBuf, _IOFBF , 1024 );
    //sleep(600);
#if 0
    if (fp)
    {
        pos = (int)ftell(fp);
        printf("1 current file pointer %d\n",pos);
        flen = fread( buff, 1, sizeof(buff), fp);
        if (flen >= 0)
        {
            printf("read %d bytes: %s\n", flen, buff);
        }
        sleep(600);
        pos = (int)ftell(fp);
        printf("2 current file pointer %d\n",pos);

        fwrite(buff1, 1, strlen(buff1), fp);
        
        pos = (int)ftell(fp);
        printf("3 current file pointer %d\n",pos);
       
        fseek(fp, 0, SEEK_SET);
        pos = (int)ftell(fp);
        printf("4 current file pointer %d\n",pos);
        
        flen = fread( buff, 1, sizeof(buff), fp);
        if (flen >= 0)
        {
            printf("read again %d bytes: %s\n", flen, buff);
        }
        pos = (int)ftell(fp);
        printf("5 current file pointer %d\n",pos);
        fclose(fp);
    }
#endif
    return 0;
}
#endif

// fopen fseek
#if 1
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_PATH "2.txt"

int main()
{
    FILE *fp = NULL;
    char buff[512] = {0};
    char buff1[32] = "hello world\n";
    char buff2[512] = {0};
    struct stat fs;
    int flen = 0;
    int pos = 0;
    stat( FILE_PATH, &fs);
    printf("file %s size %d\n", FILE_PATH, fs.st_size);

    fp = fopen(FILE_PATH, "a+");
    //setvbuf ( fp , sBuf, _IOFBF , 1024 );
    //sleep(600);
    if (fp)
    {
#if 0
        pos = (int)ftell(fp);
        printf("1 current file pointer %d\n",pos);
        flen = fread( buff, 1, sizeof(buff), fp);
        if (flen >= 0)
        {
            printf("read %d bytes: %s\n", flen, buff);
        }
        pos = (int)ftell(fp);
        printf("2 current file pointer %d\n",pos);
#endif
        fwrite(buff1, 1, strlen(buff1), fp);
        //fprintf(fp, "%s\n", buff1);
        rewind(fp);
        flen = fread( buff, sizeof(buff),1, fp);
        if (flen >= 0)
        {
            printf("read again %d bytes: %s err %d\n", flen, buff, ferror(fp));
        }
       

        pos = (int)ftell(fp);
        printf("3 current file pointer %d\n",pos);
#if 0 
        //fseek(fp, 0, SEEK_SET);
        rewind(fp);
        pos = (int)ftell(fp);
        printf("4 current file pointer %d\n",pos);
        
        flen = fread( buff, 1, sizeof(buff), fp);
        if (flen >= 0)
        {
            printf("read third %d bytes: %s\n", flen, buff);
        }
        pos = (int)ftell(fp);
        printf("5 current file pointer %d\n",pos);

        fseek(fp, -strlen(buff), SEEK_CUR);
        pos = (int)ftell(fp);
        printf("6 current file pointer %d\n",pos);

        if (fgets(buff2, 256, fp))
        {
            printf("fgets %d bytes: %s\n", strlen(buff2), buff2);
        }
#endif
        fclose(fp);
    }

    return 0;
}
#endif

#if 0
#include <stdio.h>
#include <string.h>
int main()
{
    char input_str[1280] = {0};
    int max_len = 0;
    int cur_len = 0;
    int left_index = 0;
    int right_index = 0;
    int i,j;
    printf("please input string: \n");
    scanf("%s", input_str);
    printf("Your input string is: %s\n", input_str);

    if (strlen(input_str) == 1)
        max_len = 1;
    else 
    {
        max_len = 1;
#if 0
        for (i = 0; i < strlen(input_str); i++)
        {
            for (j = i+max_len; j < strlen(input_str); j++)
            {
                int ii = i, jj = j;
                cur_len = 0;
                //printf("i %d j %d\n", i, j);
                while (jj >= ii)
                {
                    //printf(" %d %c %d %c\n", ii, input_str[ii], jj, input_str[jj]);
                    if (input_str[ii] == input_str[jj])
                    {
                        if (ii == jj)
                        {
                            cur_len++;
                        }
                        else
                        {
                            cur_len += 2;
                        }
                        ii++;
                        jj--;
                        //printf(" cur_len %d\n", cur_len);
                        continue;
                    }
                    else
                    {
                        cur_len = 0;
                        //printf(" clean cur_len %d\n", cur_len);
                        break;
                    }
                }
                if (cur_len > max_len)
                {
                    max_len = cur_len;
                    left_index = i;
                    right_index = j;
                }
                if (max_len == 0)
                {
                    max_len = 1;
                    left_index = right_index = 0;
                }
            }
        }
#endif
        {
            int len = 0;
            char tmp_str1[1024] = {0};
            for (i = 0; i < strlen(input_str); i++)
            {
                for (j = i+max_len; j < strlen(input_str); j++)
                {
                    len = j-i+1;

                }
            
            }

        }
    }
    printf("the longest plalindrome string len %d, %d %d\n", max_len, left_index, right_index);
    return 0;
}

#endif

#if 0
#include <stdio.h>
#define LOG(fmt, args...) { printf("clientdb " fmt "\n", ## args);}
int main()
{
    LOG("hello world");
    return 0;
}
#endif
#if 0 // get pid by process name
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
 
#define BUF_SIZE 1024

void getPidByName(pid_t *pid, char *task_name)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];
    char cur_task_name[50];
    char buf[BUF_SIZE];

    dir = opendir("/proc"); 
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) //循环读取/proc下的每一个文件/文件夹
        {
            //如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;
           
            sprintf(filepath, "/proc/%s/status", ptr->d_name);//生成要读取的文件的路径
            fp = fopen(filepath, "r");
            if (NULL != fp)
            {
                if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);
        
                //如果文件内容满足要求则打印路径的名字（即进程的PID）
                if (!strcmp(task_name, cur_task_name)){
                    sscanf(ptr->d_name, "%d", pid);
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
}

void getNameByPid(pid_t pid, char *task_name) {
    char proc_pid_path[BUF_SIZE];
    char buf[BUF_SIZE];

    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE* fp = fopen(proc_pid_path, "r");
    if(NULL != fp){
        if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}

void main(int argc, char** argv)
{
    char task_name[50];
    pid_t pid = getpid();

    printf("pid of this process:%d\n", pid);
    getNameByPid(pid, task_name);
    
    /*
    strcpy(task_name, argv[0]+2);
    printf("task name is %s\n", task_name);
    getPidByName(task_name);
    */
    printf("getNameByPid:%s\n", task_name);
    getPidByName(&pid, task_name);
    printf("getPidByName:%d\n", pid);
    sleep(15);
}
#endif

#if 0
#include <stdio.h>
#include <string.h>
#include <stdint.h>
typedef uint32_t UINT32;
void fun4(void)
{
  void* array[10] = {0};
  UINT32 size = 0;
  char **strframe = NULL;
  UINT32 i = 0, j = 0;

  size = backtrace(array, 10);
  strframe = (char **)backtrace_symbols(array, size);

  printf("print call frame now:\n");
  for(i = 0; i < size; i++){
    printf("frame %d -- %s\n", i, strframe[i]);
  }

  if(strframe)
  {
    free(strframe);
    strframe = NULL;
  }
}

void fun3(void)
{
  fun4();
}

void fun2(void)
{
  fun3();
}

void fun1(void)
{
  fun2();
}

int main(void)
{
  fun1();
  return 0;
}

#endif








#if 0 // thread lock
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
 
int main(void)
{
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
	//1
	pthread_mutex_lock(&lock);
	printf("mutex is locked.\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);   //strftime(char *str, size_t maxsize, const char *fmt, struct tm *time)
											 //按照参数fmt所设定格式将time类型的参数格式化为日期时间信息，然后存储在字符串str中(至多maxsize 个字符)
											 //参考http://ganquan.info/standard-c/function/strftime
	printf("Current time is %s.\n", buf);
	tout.tv_sec += 10;   //延迟10s
	
	//2
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("The time is now %s\n", buf);
	
	if(err == 0)
	{
		printf("mutex locked again!\n");
	}
	else
	{
		printf("Can't lock mutex again: %s\n", strerror(err));
	}
	
	return 0;
}

#endif
#if 0
#include <stdio.h>
int main()
{
    char cmd[256] = {0};
    int a = sprintf(cmd, "test ");
    printf("%d\n", a);
    sprintf(cmd+a, "fuck \n");
    printf("%s", cmd);
    return 0;
}
#endif

#if 0 // redirect stdout to file
#include <stdio.h>
int main()
{
    freopen("test1.txt", "w+", stdout); 
    printf("test printf redirect\n");
    system("dmesg|grep IPv6:");
    freopen("/dev/tty", "w+", stdout); 
    printf("printf back to stdou: test printf redirect\n");
    return 0;
}
#endif

#if 0
#include <stdio.h>
#include <unistd.h>

int main()
{
    int size = 0, i = 0;
    char *p = malloc(1024);

    *((int *)p) = 0x38383838;
    *((int *)p+1) = 0x39393939;
    printf("%x\n", *(int *)p);
    printf("%x\n", *((int *)p+1));

    for (i=0; i < 9; i++)
    {
        printf("%x\n", *p);
        p++;
    }
    size = getpagesize();
    printf("page size is %d\n", size);
    return 0;
}
#endif

#if 0 // high performance for udp socket

#include <stdio.h>
#include <sys/types.h>
#define __USE_GNU
#include <sys/socket.h>
#include <netinet/in.h>

#define VLAN 10
#define BUFSIZE 200

int main()
{
    int fd = 0, i = 0;
    int ret = 0;
    struct sockaddr_in addr = {0};
    struct mmsghdr msgs[VLAN];
    struct iovec iovecs[VLAN];
    char bufs[VLAN][BUFSIZE+1];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd <= 0)
    {
        printf("socket failed, erron: %d\n", fd);
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(4501);
    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret != 0)
    {
        printf("bind failed, erron: %d\n", ret);
        return 0;
    }
   
    for (i = 0; i < VLAN; i++)
    {
        iovecs[i].iov_base = bufs[i];
        iovecs[i].iov_len = BUFSIZE;
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
    }
    while (1)
    {
        ret = recvmmsg(fd, &msgs, VLAN, 0, 0);
        printf("recv msg %d\n", ret);
    }
    close(fd);
    return 0;
}
#endif

#if 0 // mem
#include <unistd.h>
#include <stdio.h>
#define MEM_LEN   1024*1024*1024
int main()
{
    char *p = (char *)malloc(MEM_LEN);
    pid_t pid;
    if (p == NULL)
    {
        printf("malloc failed.\n");
        return -1;
    }

    memset(p, 1, 1024*1024);

    pid = fork();
    if (pid == 0)
    {
        printf("son process pid %d\n", getpid());
        sleep(100);
        exit(0);
    }
    else
    {
        printf("par process pid %d\n", getpid());
        wait(NULL);
    }

    if (p)
        free(p);
    return 0;
}
#endif



#if 0 //sigsuspend
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void oops(void *msg){  
    perror(msg);  
    exit(1);  
}  

void deal()
{
   pr_mask("signal processing....");
}

void pr_mask( const char *str )
{  
    sigset_t set;  
    int errno_save;         //get the pre errno  
    errno_save = errno;  
  
    if( sigprocmask( 0, NULL , &set ) == -1 )  
        oops( " sigmask" );  
    else{  
        printf( "%s: " , str );  
        if( sigismember( &set , SIGQUIT ) == 1 )  
            printf( " SIGQUIT" );  
        if( sigismember( &set , SIGINT ) == 1)  
            printf( " SIGINT" );  
        if( sigismember( &set , SIGUSR1 ) == 1)  
            printf( " SIGUSR1" );  
        if( sigismember( &set , SIGALRM ) == 1)  
            printf( " SIGALRM" ); 
        printf("\n"); 
    }  
    errno = errno_save ;  
}  

int main()
{
    sigset_t  sigs,sigmask;
    int  i, ret = 0;
    signal(SIGINT, deal);
    sigemptyset(&sigs);
    sigemptyset(&sigmask);
    ret = sigaddset(&sigs, SIGINT);
    printf("add SIGINT to sigset, ret %d, err: %s\n", ret, strerror(errno));
    ret = sigprocmask(SIG_BLOCK, &sigs, 0);
    printf("block int signal, ret %d\n", ret);
    pr_mask("current block mask");
    for(i=0 ; i<3 ; i++)
    {
       printf("task begin:\n");    //模拟业务处理
       sleep(3);
       printf("task end:\n"); 
       sigsuspend(&sigmask);       //处理正在排队的信号，处理信号完毕后，
                                   //sigsuspend函数才返回，并执行下个业务处理
       printf("task begin xx:\n");    //模拟业务处理
       sleep(3);
       printf("task end xx:\n");    //模拟业务处理
    }   

    return 0; 
}   
#endif

#if 0 // signal
#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
    printf("SIGINT sig\n");
}

int main()
{
    sigset_t new, old;
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, 0);
    
    sigemptyset(&new);
    sigaddset(&new, SIGINT);
    sigprocmask(SIG_BLOCK, &new, &old);
    printf("Blocked\n");
    sigprocmask(SIG_SETMASK, &old, NULL);
    pause();
   
    return 0;
}

#endif

#if 0
#include <stdio.h>
#include <stddef.h>
#include <time.h>
int main(void)
{
    time_t timer;
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    printf("Local time is: %s\n", asctime(tblock));
    return 0;
}
#endif

#if 0 // time funcs
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(void)
{
    struct timeval timeval_1;
    struct timespec timeval_2;
    struct timespec timeval_3;
    struct timespec timeval_4;
    time_t timeval;

    timeval = time(NULL); // seconds from 19700101
    printf("time() %ld\n", timeval);

    gettimeofday(&timeval_1, NULL); // same with time(), but can get us
    printf("gettimeofday() %ld: %ld\n", timeval_1.tv_sec, timeval_1.tv_usec);

    clock_gettime(CLOCK_REALTIME, &timeval_2); // from 19700101
    printf("clock_gettime() %ld: %ld\n", timeval_2.tv_sec, timeval_2.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &timeval_3); // /proc/uptime 
    printf("clock_gettime() %ld: %ld\n", timeval_3.tv_sec, timeval_3.tv_nsec);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeval_4);  
    printf("clock_gettime() %ld: %ld\n", timeval_4.tv_sec, timeval_4.tv_nsec);
    return 0;
}

#endif

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

int do_checksum(unsigned char *buff, int len)
{
    int count = len;
    unsigned int sum = 0, sum1 = 0;
    while (count > 1)
    {
        sum += htons(*((unsigned short *)buff));   // the return of cast is host order
        printf("sum %x\n", sum);
        buff+=2;
        count -= 2;
    }

    if (count > 0)
    {
        sum += *buff; 
    }

    if (sum >> 16)
    {
        sum1 = sum & 0xffff;
        sum1 += sum >> 16;
    }

    return ~sum1;
}


int main()
{
    uchar ipheader[] = {0x45,0x00,0x00,0xa2,0x1d,0x8d,0x40,0x00,0x80,0x06,0x00,0x00,0x0a,0x58,0x46,0x40,0x0a,0x58,0x46,0x1e};
    uchar tcpcontext[] = {0xe1,0x21,0x00,0x8b,0x43,0x72,0x30,0xa0,0xdd,0x18,0x5f,0x09,0x50,0x18,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xff,0x53,0x4d,0x42,0x25,0x00,0x00,0x00,0x00,0x18,0x07,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x28,0x04,0x00,0x08,0x40,0x01,0x0e,0x1a,0x00,0x00,0x00,0x08,0x00,0x16,0x11,0x00,0x00,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x1a,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x37,0x00,0x00,0x5c,0x00,0x50,0x00,0x49,0x00,0x50,0x00,0x45,0x00,0x5c,0x00,0x4c,0x00,0x41,0x00,0x4e,0x00,0x4d,0x00,0x41,0x00,0x4e,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x57,0x72,0x4c,0x65,0x68,0x44,0x4f,0x00,0x42,0x31,0x36,0x42,0x42,0x44,0x7a,0x00,0x01,0x00,0x16,0x11,0x00,0x00,0x00,0x80};

    uchar iphdr[] = {0x45, 0x10, 0x00, 0x28, 
                     0xa9, 0xc9, 0x40, 0x00, 
                     0x3c, 0x06, 0x00, 0x00, 
                     0x0a, 0x59, 0xff, 0x94, 
                     0x0a, 0x58, 0x26, 0x1b};
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
    
    checksum = doChecksum(iphdr, 20);
    printf("ip checksum is 0x%x\n", checksum);
    
    checksum = do_checksum(iphdr, 20);
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
#include <sys/file.h>

int main()
{
	int fd, i;
	char path[] = "/home/wt/code/git/file.txt";
	extern int errno;

	fd = open(path, O_WRONLY | O_CREAT);
	if (fd != -1)
	{
		printf("open file %s .\n", path);
		printf("please input a number to lock the file.\n");
		scanf("%d", &i);
		printf("try lock file:%s...\n", path);

		if (flock(fd, LOCK_SH) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}

                //exit(0);
#if 0
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
#endif
                sleep(100000);
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

#if 0  // flock between process and its child process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int fd = 0;
int fd2 = 0;
int fd3 = 0;
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

#if 0 // flock
		if (flock(fd, LOCK_EX) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}
#endif
#if 1 // fcntl
{
		struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;   
        lock.l_pid = getpid();

		if (fcntl(fd, F_SETLKW, &lock) == 0)
		{
			printf("the file was locked.\n");
		}
		else
		{
			printf("the file was not locked.\n");
		}
}
#endif
        //close(fd);

#if 0 // nest flock
	    fd3 = open(path, O_WRONLY | O_CREAT);
		printf("twice: try lock file:%s...\n", path);
		if (flock(fd3, LOCK_EX) == 0)
		{
			printf("the file was locked again.\n");
		}
		else
		{
			printf("the file was not locked again.\n");
		}
#endif
		int pid;

		if ((pid = fork()) < 0)
		{
			printf("fork failed\n");
			exit(0);
		}
		else if (pid == 0)
		{		// child
			//sleep(5);
            fd2 = open(path, O_WRONLY | O_CREAT);
                        //close(fd);
		    printf("child tries to lock file...\n");
		        //printf("child unlock file...\n");
#if 0
			if(flock(fd2,LOCK_EX)==0) 
            {
			    printf("child add ex success\n");
			} 
            else 
            { 
			    printf("child add ex failed\n");
			}
            close(fd2);
			printf("child close\n");
#endif
        {
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;   
            lock.l_pid = getpid();

            if (fcntl(fd2, F_SETLKW, &lock) == 0)
            {
                printf("the file was locked.\n");
            }
            else
            {
                printf("the file was not locked.\n");
            }
        }
#if 0
			if (flock(fd, LOCK_UN) == 0)
			{
				printf("child unlock success.\n");
			}
			else
			{
				printf("child unlock fail.\n");
			}
#endif
			sleep(5000);
            return 0;
		}
		else
		{		// parent
			//int pid2 = wait(NULL);
            sleep(300);
            //flock(fd, LOCK_UN);
			//printf("father unlock\n");
            close(fd);
			printf("father close\n");
			//printf("the file was unlocked.\n");
                        //flock(fd, LOCK_UN);
            sleep(5000);
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

#if 0			//flock
#include <stdio.h>
#include <pthread.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

void readfile_thread(char *filepath)
{
    printf("thread process : %d %d\n", getpid(), gettid());
	
    int fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		printf("read file error, %s.\n", strerror(errno));
		return;
	}
	flock(fd, LOCK_EX);
	printf("thread lock the file for read.\n");
	flock(fd, LOCK_EX);
	printf("thread lock the file for read again.\n");
	sleep(10);
	printf("thread end.\n");
	close(fd);
	return;
}

int main()
{
	char filepath[] = "/home/wt/code/git/tmp.txt";
	pthread_t thread_id;

    printf("main process : %d %d\n", getpid(), gettid());
	pthread_create(&thread_id, NULL, readfile_thread, filepath);

	sleep(3);
	int fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		printf("read file error, %s.\n", strerror(errno));
		return;
	}
	flock(fd, LOCK_EX);
	printf("main process lock the file for read.\n");
	//pthread_create(&thread_id, NULL, readfile_thread, filepath);
    sleep(10000);
	printf("main process end.\n");
    close(fd);
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


#if 0 // system() issue 2: wait() in signal handler will cause the parent process to get stuck.
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

//this will be triggered by system .
void sig_chld(int signo) 
{
    pid_t pid;
    int stat;

    printf("sig handler enter pid %d.\n", getpid());
    //system("who");
    //signal(SIGCHLD, SIG_DFL);

    //while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    while ( (pid = wait(&stat)) > 0) 
    {
        printf("child %d terminated\n", pid);
    }
    printf("sig handler exit pid %d.\n", getpid());
    return;
}

int main()
{
    pid_t pid;
    pid = fork();
    if (pid > 0)
    {
        printf("parent process start pid %d.\n", getpid());
        signal(SIGCHLD, sig_chld);
        sleep(3);
        system("echo 123");
        while (1)
        {
            sleep(1);
            printf("parent process loops to do something.\n");
        }
        printf("parent process exit pid %d.\n", getpid());
    }
    else if (pid == 0)
    {
        printf("son process begin pid %d.\n", getpid());
        sleep(300);
        printf("son process exit pid %d.\n", getpid());
        return 0;
    }
    return 0;
}
#endif

#if 0
#include <pwd.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
static void my_alarm(int signo)
{
        struct passwd *rootptr;
        char *p = NULL;
 
        printf("in signal handler\n");
        //if ((rootptr = getpwnam("root")) == NULL)
        if ((p = malloc(128)) == NULL)
        {
                printf("getpwnam(root) error\n");
                return;
        }
        memset(p, 0, 128);
        alarm(1);
        if (p)
        {
                free(p);
        }
        return;
}

unsigned long num = 0; 

int main(void)
{
        struct passwd *ptr;
        char *p = NULL;
 
        signal(SIGALRM, my_alarm);
        alarm(1);
        for ( ; ; )
        {
                if ((p = malloc(128)) == NULL)
                //if ((ptr = getpwnam("sebastien")) == NULL)
                {
                        //printf("getpwnam error\n");
                        continue;
                }
                //if (strcmp(ptr->pw_name, "sebastien") != 0)
                memset(p, 0, 128);
                if (strcpy(p, "hello world.") != 0)
                {
                        printf("return value corrupted!, %lu %p\n", num++, p);
                }
                if (p)
                {
                        free(p);
                }
        }
        return 0;
}
#endif


#if 0 // signal pignbi
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_usr1(int signo)
{
  printf("SIGUSR1 function\n");
}
static void sig_usr2(int signo)
{
  printf("SIGUSR2 function\n");
}

static void sig_int(int signo)
{
  printf("SIGINT function\n");
}

void sig_catch(int sig_no, void (*f)(int))
{
    struct sigaction sa;
    sa.sa_handler = f;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(sig_no, &sa, (struct sigaction *) 0);
}


int main()
{
  sigset_t newmask,oldmask;
  
  sig_catch(SIGUSR1,sig_usr1);
  sig_catch(SIGUSR2,sig_usr2);
  sig_catch(SIGINT,sig_int);
   
//  signal(SIGUSR1,sig_usr1);
//  signal(SIGUSR2,sig_usr2);
//  signal(SIGINT,sig_int);
      
  sigemptyset(&newmask);
  sigaddset(&newmask,SIGUSR1);
  sigaddset(&newmask,SIGUSR2);
  sigaddset(&newmask,SIGINT);
  
  sigprocmask(SIG_BLOCK,&newmask,&oldmask);
  printf("SIGUSR is blocked\n");

  kill(getpid(),SIGUSR2);
  kill(getpid(),SIGUSR1);
  kill(getpid(),SIGUSR2);
  kill(getpid(),SIGUSR1);
  kill(getpid(),SIGUSR2);
  kill(getpid(),SIGUSR1);
  kill(getpid(),SIGINT);
  printf("SIGUSR is unblocked\n");    

  //方法一    
  sigprocmask(SIG_UNBLOCK,&newmask,NULL);
  
  //方法二
  //sigprocmask(SIG_SETMASK,&oldmask,NULL);
  
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

	fp = fopen("/home/wt/code/git/xx/txt", "a");
	if (fp != NULL)
	{
        printf("hello world.\n");
		while (fgets(cmd, sizeof(cmd), fp) != NULL)
		{
			printf("%s\n", cmd);
		}
		fclose(fp);
	}

	fp2 = fopen("txt1", "a");
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
