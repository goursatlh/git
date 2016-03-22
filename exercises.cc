#if 1
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
