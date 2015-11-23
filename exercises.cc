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

    fd=open("test.txt",O_RDONLY);//打开已存在文件test.txt
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
    fstat(fd,&buf);
    printf("7.link=%d\n",buf.st_nlink);//测试链接数

    close(fd);
    fstat(fd,&buf);
    printf("8.link=%d\n",buf.st_nlink);//测试链接数
    
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
