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
    /*�򿪣����ߴ�����һ���ļ���Ȼ��ʹ��setbuf����Ϊnobuf����������ر�ǰ�������*/
    if((fp = fopen("no_buf1.txt","w")) == NULL)
    {
            perror("file open fail");
            exit(-1);
    }
    setbuf(fp,NULL);
    fwrite(msg1,7,1,fp);
    printf("test setbuf(no buf)!check no_buf1.txt\n");//�鿴 buf ��� 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//�ر�������˽���д buf������� buf �Ļ���
    /*�򿪣����ߴ�����һ���ļ���Ȼ��ʹ�� setvbuf ����Ϊ nobuf ����������ر�ǰ�������*/ 
    if((fp = fopen("no_buf2.txt","w")) == NULL)
    {
            perror("file open failure!");
            exit(-1);
    }
    setvbuf(fp,NULL,_IONBF,0);         //����Ϊ�� buf 
    fwrite(msg1,7,1,fp);
    printf("test setbuf(no buf)!check no_buf1.txt\n");        //�鿴 buf ��� 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);//�ر�������˽���д buf ( ����� buf �Ļ�)
    if((fp = fopen("1_buf.txt","w")) == NULL)
    {
            perror("fail open file");
            exit(-1);
    }
    setvbuf(fp,buf,_IOLBF,sizeof(buf));        //����Ϊ�� buf
    fwrite(msg2,sizeof(msg2),1,fp);        //д����
    printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");        //�鿴 buf ��� 
    printf("press enter to continue!\n");
    getchar();
    fclose(fp);        //�ر�������˽���д buf 
    //�򿪣����ߴ�����һ���ļ���Ȼ��ʹ�� setvbuf ����Ϊȫ buf����������ر�ǰ������� 
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
    fclose(fp);//�ر�������˽���д buf 
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
    printf("1.link=%d\n",buf.st_nlink);//δ���ļ�֮ǰ����������
    fd=open("test.txt",O_CREAT);//���Ѵ����ļ�test.txt
    stat("test.txt",&buf);
    printf("2.link=%d\n",buf.st_nlink);//����������

    close(fd);//�ر��ļ�test.txt
    stat("test.txt",&buf);
    printf("3.link=%d\n",buf.st_nlink);//����������

    link("test.txt","test2.txt");//����Ӳ����test2.txt
    stat("test.txt",&buf);
    printf("4.link=%d\n",buf.st_nlink);//����������

    unlink("test2.txt");//ɾ��test2.txt
    stat("test.txt",&buf);
    printf("5.link=%d\n",buf.st_nlink);//����������

    fd=open("test.txt",O_RDONLY);//���Ѵ����ļ�test.txt
    stat("test.txt",&buf);
    printf("6.link=%d\n",buf.st_nlink);//����������

    unlink("test.txt");//ɾ��test.txt
    fstat(fd,&buf); //use fstat not stat
    printf("7.link=%d\n",buf.st_nlink);//����������

    close(fd);
    fstat(fd,&buf);
    printf("8.link=%d\n",buf.st_nlink);//����������
    
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
    }//����unlink��0��Ŀ¼��û���ļ��ˣ�����fd�����Է����������ݣ�����Ϊ�����̻�û��close�����close֮���ɶ��û��~

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

    //�˴����Ǹ���fstat��������stat����Ϊunlilnk�Ѿ�ɾ���ļ��������Բ�����ͨ���ļ�������
    //����fd��Ȼ�Ǵ��ŵģ��ļ����ݻ�û�б�����ɾ�������ɿ���ʹ��fd����ļ���Ϣ
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
