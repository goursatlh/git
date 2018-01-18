#if 1 // get/set system time

#include <time.h>
#include <stdio.h>

int main()
{
    struct timespec res;

    clock_gettime(CLOCK_MONOTONIC, &res);
    printf("time %u %u\n", res.tv_sec, res.tv_nsec);

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
        MOV spReg, __current_sp()
        MOV pcReg, __current_pc()
        MOV lrReg, __return_address()
    }
    printf("SP = 0x%X\n",spReg);
    printf("PC = 0x%X\n",pcReg);
    printf("LR = 0x%X\n",lrReg);
}
#endif

void segment_handle(int signum, siginfo_t* siginfo, void* context)
{
    char *signame;
    ucontext_t* ucontext;
    struct sigcontext* usigcontext;

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
    
    ucontext = (ucontext_t*) context;
    usigcontext = (struct sigcontext*) &(ucontext->uc_mcontext);
    if ((signum == SIGSEGV) || (signum == SIGFPE))
    {
        printf( "sig %s: segment fault addr=%016lx, cs=%016lx, pc=%016lx, lr/r14=%016lx, r13=%016lx\n", 
                signame, (long)(siginfo->si_addr), usigcontext->cs, usigcontext->rip, usigcontext->r14, usigcontext->r13 ); 
    }
    //printReg();
    
    exit(0); // why can't use return: exit() end of process;
}

int main()
{
    //int *p = NULL;
    char a[3] = {0};
    int ret = 0;
    struct sigaction sigact;
    sigact.sa_sigaction = segment_handle;
    sigact.sa_flags = SA_SIGINFO;
    ret = sigaction(SIGSEGV, &sigact, NULL);
    ret = sigaction(SIGFPE, &sigact, NULL);
    //a[8000] = 1; // why 8000 seg, 7000 works well
    //printf("log %d\n", log);
    ret = a[1]/0;
    return 0;
}

#endif
#if 0 // buff for file io 
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
    char msg[128] = {0};
    char buf[128];
    
    if((fp = fopen("no_buf1.txt","w")) == NULL)
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
    printf("test setbuf(no buf)!check no_buf1.txt\n");//查看 buf 情况
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf（如果有 buf 的话）

    /* 打开（或者创建）一个文件，然后使用 setvbuf 设置为 nobuf 情况，并检查关闭前流的情况 */
    if((fp = fopen("no_buf2.txt","w")) == NULL)
    {
        perror("file open failure!");
        exit(-1);
    }
    setvbuf(fp,NULL,_IONBF,0);         //设置为无 buf
    fwrite(msg1,7,1,fp);
    printf("test setbuf(no buf)!check no_buf2.txt\n");        //查看 buf 情况
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf( 如果有 buf 的话)

    if((fp = fopen("1_buf.txt","w")) == NULL)
    {
        perror("fail open file");
        exit(-1);
    }
    setvbuf(fp,buf,_IOLBF,sizeof(buf));        //设置为行 buf
    fwrite(msg2,sizeof(msg2),1,fp);        //写内容
    printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");        //查看 buf 情况
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);        //关闭流，因此将回写 buf

    //打开（或者创建）一个文件，然后使用 setvbuf 设置为全 buf情况，并检查关闭前流的情况
    if((fp = fopen("f_buf.txt","w")) == NULL)
    {
        perror("file open failure!");
        exit(-1);
    }
    setvbuf(fp,buf,_IOFBF,sizeof(buf));
    for(i = 0;i < 2;i++)
    {
        fputs(msg1,fp);
    }
    printf("test setbuf(full buf)! check f_buf.txt\n");
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf
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
    char cmd[128] = {0};

    
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

void print_info(
        char*      var_name,
        char*      var_ptr,
        size_t     size_in_kb
)
{
    printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n",
                               var_name, size_in_kb, var_ptr, sbrk(0));
}

int main(int argc, char** argv)
{
        char *heap_var1, *heap_var2, *heap_var3 ;
        char *mmap_var1, *mmap_var2, *mmap_var3 ;
        char *maybe_mmap_var;
        printf("Orginal heap top is 0x%lx\n", sbrk(0));
        heap_var1 = malloc(32*1024);
        print_info("heap_var1", heap_var1, 32);
        heap_var2 = malloc(64*1024);
        print_info("heap_var2", heap_var2, 64);
        heap_var3 = malloc(127*1024);
        print_info("heap_var3", heap_var3, 127);
        printf("\n");
        maybe_mmap_var = malloc(128*1024);
        print_info("maybe_mmap_var", maybe_mmap_var, 128);
        //mmap
        mmap_var1 = malloc(128*1024);
        print_info("mmap_var1", mmap_var1, 128);
        // set M_MMAP_THRESHOLD to 64k
        mallopt(M_MMAP_THRESHOLD, 64*1024);
        printf("set M_MMAP_THRESHOLD to 64k\n");
        mmap_var2 = malloc(64*1024);
        print_info("mmap_var2", mmap_var2, 64);
        mmap_var3 = malloc(127*1024);
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
void print_info(
        char*      var_name,
        char*      var_ptr,
        size_t     size_in_kb
)

{
        printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n",
                 var_name, size_in_kb, var_ptr, sbrk(0));
        system(ps_cmd);
}

int main(int argc, char** argv)
{
        char *non_set_var, *set_1k_var, *set_5k_var, *set_7k_var;
        pid_t pid;
        pid = getpid();
        sprintf(ps_cmd, "ps aux | grep %lu | grep -v grep|awk '{print \"VSZ: \"$5\" RSS: \" $6}'", pid);
        non_set_var = malloc(32*1024);
        print_info("non_set_var", non_set_var, 32);
        set_1k_var = malloc(64*1024);
        memset(set_1k_var, 0, 1024);
        print_info("set_1k_var", set_1k_var, 64);
        set_5k_var = malloc(127*1024);
        memset(set_5k_var, 0, 5*1024);
        print_info("set_5k_var", set_5k_var, 127);
        set_7k_var = malloc(64*1024);
        memset(set_1k_var, 0, 7*1024);
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

size_t  heap_malloc_total, heap_free_total, mmap_total, mmap_count;

void print_info()
{
        struct mallinfo mi = mallinfo();
        printf("count by itself:\n");
        printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
                heap_malloc_total*1024, heap_free_total*1024, heap_malloc_total*1024 - heap_free_total*1024,
                mmap_total*1024, mmap_count);
        printf("count by mallinfo:\n");
        printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu, %lu, %lu, %lu\n",
                mi.arena, mi.fordblks, mi.uordblks,
                mi.hblkhd, mi.hblks, mi.fsmblks, mi.smblks, mi.ordblks);
        printf("from malloc_stats:\n");
        malloc_stats();
}

#define ARRAY_SIZE 200
int main(int argc, char** argv)
{
        char** ptr_arr[ARRAY_SIZE];
        int i;
        for( i = 0; i < ARRAY_SIZE; i++) {
                ptr_arr[i] = malloc(i * 1024);
                if ( i < 128)
                        heap_malloc_total += i;
                else {
                        mmap_total += i;
                        mmap_count++;
                }

        }
        print_info();
        for( i = 0; i < ARRAY_SIZE; i++) {
                if ( i % 2 == 0)
                        continue;
                free(ptr_arr[i]);
                if ( i < 128)
                        heap_free_total += i;
                else {
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
    pc1 = strtok_r(aa, ";",&pc2); 
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

struct A{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
};

struct B{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
}__attribute__((aligned));

struct C{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
}__attribute__((aligned(1)));

struct D{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
}__attribute__((aligned(4)));

struct E{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
}__attribute__((aligned(8)));

struct F{
  char a;
  int b;
  unsigned short c;
  long d;
  unsigned long long e;
  char f;
}__attribute__((packed));

int main(int argc, char **argv)
{
  printf("long %d, longlong %d, short %d, int %d\n", sizeof(long), sizeof(unsigned long long), sizeof(short), sizeof(int));
  printf("A = %d, B = %d, C = %d, D = %d, E = %d, F = %d\n",
  sizeof(struct A), sizeof(struct B), sizeof(struct C), sizeof(struct D), sizeof(struct E), sizeof(struct F));
  printf("%x %x %x %x %x %x\n", &(((struct A *)0)->a), &(((struct A *)0)->b), &(((struct A *)0)->c), 
                                &(((struct A *)0)->d), &(((struct A *)0)->e), &(((struct A *)0)->f));
  printf("%x %x %x %x %x %x\n", &(((struct B *)0)->a), &(((struct B *)0)->b), &(((struct B *)0)->c), 
                                &(((struct B *)0)->d), &(((struct B *)0)->e), &(((struct B *)0)->f));
  return 0;
}

#endif

#if 0 // variables in the memory layout
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

    printf("%p\n", p);                //constant area
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
    /*打开（或者创建）一个文件，然后使用setbuf设置为nobuf情况，并检查关闭前流的情况*/
    if((fp = fopen("no_buf1.txt","w")) == NULL)
    {
            perror("file open fail");
            exit(-1);
    }
    setbuf(fp,NULL);
    fwrite(msg1,7,1,fp);
    printf("test setbuf(no buf)!check no_buf1.txt\n");//查看 buf 情况 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf（如果有 buf 的话）
    /*打开（或者创建）一个文件，然后使用 setvbuf 设置为 nobuf 情况，并检查关闭前流的情况*/ 
    if((fp = fopen("no_buf2.txt","w")) == NULL)
    {
            perror("file open failure!");
            exit(-1);
    }
    setvbuf(fp,NULL,_IONBF,0);         //设置为无 buf 
    fwrite(msg1,7,1,fp);
    printf("test setbuf(no buf)!check no_buf1.txt\n");        //查看 buf 情况 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf ( 如果有 buf 的话)
    if((fp = fopen("1_buf.txt","w")) == NULL)
    {
            perror("fail open file");
            exit(-1);
    }
    setvbuf(fp,buf,_IOLBF,sizeof(buf));        //设置为行 buf
    fwrite(msg2,sizeof(msg2),1,fp);        //写内容
    printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");        //查看 buf 情况 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);        //关闭流，因此将回写 buf 
    //打开（或者创建）一个文件，然后使用 setvbuf 设置为全 buf情况，并检查关闭前流的情况 
    if((fp = fopen("f_buf.txt","w")) == NULL)
    {
            perror("file open failure!");
            exit(-1);
    }
    setvbuf(fp,buf,_IOFBF,sizeof(buf));
    for(i = 0;i < 2;i++)
    {
            fputs(msg1,fp);
    }
    printf("test setbuf(full buf)! check f_buf.txt\n");
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//关闭流，因此将回写 buf 
    return 0;
}

#endif

#if 0 // how long is the printf buff
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    int c = atoi(argv[1]); // 1024 no output; 1025 output
    while (c-- > 0)
        printf("a");
    while (1);
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
    function(1,2,3);
    x = 1;

    printf("%d\n", x);
    return 0;
}
#endif

#if 0 //vfork
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
        return 0;
        //exit(0);
    }
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
    stat("test.txt",&buf);
    printf("1.link=%d\n",buf.st_nlink);//未打开文件之前测试链接数
    fd=open("test.txt",O_CREAT);//打开已存在文件test.txt
    stat("test.txt",&buf);
    printf("2.link=%d\n",buf.st_nlink);//测试链接数

    close(fd);//关闭文件test.txt
    stat("test.txt",&buf);
    printf("3.link=%d\n",buf.st_nlink);//测试链接数

    link("test.txt","test2.txt");//创建硬链接test2.txt
    stat("test.txt",&buf);
    printf("4.link=%d\n",buf.st_nlink);//测试链接数

    unlink("test2.txt");//删除test2.txt
    stat("test.txt",&buf);
    printf("5.link=%d\n",buf.st_nlink);//测试链接数

    fd=open("test.txt",O_RDONLY);//打开已存在文件test.txt
    stat("test.txt",&buf);
    printf("6.link=%d\n",buf.st_nlink);//测试链接数

    unlink("test.txt");//删除test.txt
    fstat(fd,&buf); //use fstat not stat
    printf("7.link=%d\n",buf.st_nlink);//测试链接数

    close(fd);
    fstat(fd,&buf);
    printf("8.link=%d\n",buf.st_nlink);//测试链接数
    
    return 0;
}
#endif

#if 0 // open,link,close,unlink
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

    if((fd=open("temp.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))<0)
    {
        printf("create file error!\n");
    }
    if(write(fd,"temp",5)<0)
    {
        printf("write wrror!\n");
    }//后面unlink到0后，目录里没有文件了，但是fd还可以访问其中内容！，因为本进程还没有close，如果close之后就啥都没鸟~

    stat("temp.txt",&buff); 
    printf("temp.link=%d\n",buff.st_nlink); 
    link("temp.txt","test.txt"); 
    stat("test.txt",&buff); 
    printf("after link the tem.link =%d\n",buff.st_nlink); 
    if(unlink("test.txt")<0)
    { 
        printf("unlink error !\n"); 
    } 
    stat("temp.txt",&buff); 
    printf("after unlink tem.link=%d\n",buff.st_nlink); 
    if(unlink("temp.txt")<0)
    { 
        printf("unlink error !\n");
    }

    //此处我们改用fstat函数而非stat，因为unlilnk已经删除文件名，所以不可以通过文件名访问
    //但是fd仍然是打开着的，文件内容还没有被真正删除，依旧可以使用fd获得文件信息
    fstat(fd,&buff);
    printf("after unlink tem.link=%u\n",buff.st_nlink);

    if((lseek(fd,0,SEEK_SET))==-1)
    {
        printf("lseek error!\n");
    }
   
    //why can read after the link number is 0
    if((read(fd,buf,5))<0)
    {
        printf("read error!\n");
    }
    printf("%s,then ,close fd ,reopen \n",buf);
    close(fd);
    
    if((lseek(fd,0,SEEK_SET))==-1)
    {
        printf("lseek error!\n");
    }

    memset(buf, 0, sizeof(buf));
    if((read(fd,buf,5))<0)
    {
        printf("read error!\n");
    }
    printf("%s after reopen \n",buf);
    return 0; 
}

#endif

#if 0 // 3.13
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main(void)
{
    vector<int> num, sum;
    int tmp = 0;
    int count = 0;
    int index = 0;
    int i = 0;

    cout<<"please input numbers: "<<endl;
    while (cin>>tmp)
    {
        num.push_back(tmp);
        if ((count >= 2) && ((count % 2) == 0))
        {
            sum[index++] = num[count-2] + num[count-1];
        }
        cin>>num[count++];
    }
    --count;

    if ((count % 2) != 0)
    {
        cout<<"Total number is "<<count<<" the last one is "<<num[count-1]<<endl; 
    }

    for (i = 0; i< count; i++)
    {
        cout<<num[i]<<" ";
    }

    cout<<endl;
    
    cout<<"use iterator to walk vector<int> type: "<<endl;
    for (vector<int>::iterator iter = num.begin(); iter != num.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    
    for (i = 0; i < index; i++)
    {
        cout<<sum[i]<<"   ";
    }
    cout<<endl;
    
    return 0;
}
#endif
