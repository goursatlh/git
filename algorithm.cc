#if 1 //string sort
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;

void sort(String[] a, int W) 
{   // Sort a[] on leading W characters. 
    int N = a.length; 
    int R = 256; 
    string[] aux = new String[N]; 

    for (int d = W-1; d >= 0; d--) 
    { // Sort by key-indexed counting on dth char. 

        int[] count = new int[R+1];               // Compute frequency counts. 
        for (int i = 0; i < N; i++) 
            count[a[i].charAt(d) + 1]++; 

        for (int r = 0; r < R; r++)               // Transform counts to indices. 
            count[r+1] += count[r]; 

        for (int i = 0; i < N; i++)               // Distribute. 
            aux[count[a[i].charAt(d)]++] = a[i]; 

        for (int i = 0; i < N; i++)               // Copy back. 
            a[i] = aux[i]; 
    } 
} 

int main()
{
    vector<string> a = {"hello", "world", "fuck2", "you23", "other"};
    sort(a, 5);
    for (int i = 0; i < a.size(); i++)
    {
        cout<<a[i]<<endl;
    }

    return 0;
}

#endif
#if 0//
#include<stdio.h>
#include<stdlib.h>
typedef int DataType;
typedef struct treenode *TreeNode;
typedef struct treenode *BinTree;

struct treenode
{
    DataType element;
    TreeNode llink;
    TreeNode rlink;
};

BinTree createBinTree(DataType x)
{
    BinTree tree = NULL;
    tree = malloc(sizeof(struct treenode));
    tree->element = x;
    return tree;
}

BinTree addToLeft(BinTree t, DataType x)
{
    TreeNode node = NULL;
    node = malloc(sizeof(struct treenode));
    if (node != NULL)
        node->element = x;
    t -> llink = node;
    return node;
}

BinTree addToRight(BinTree t, DataType x)
{
    TreeNode node = NULL;
    node = malloc(sizeof(struct treenode));
    if (node != NULL)
        node->element = x;
    t-> rlink = node;
    return node;
}

void visitRoot(BinTree tree)
{
    printf("%d ", tree->element);
}

BinTree leftChild(BinTree tree)
{
    return tree->llink;
}

BinTree rightChild(BinTree tree)
{
    return tree->rlink;
}

void preOrder(BinTree tree)
{
    if(tree == NULL)
        return;
    visitRoot(tree);
    preOrder(leftChild(tree));
    preOrder(rightChild(tree));
}

void inOrder(BinTree tree)
{
    if(NULL == tree)
        return;
    inOrder(leftChild(tree));
    visitRoot(tree);
    inOrder(rightChild(tree));
}

void postOrder(BinTree tree)
{
    if(NULL == tree)
        return;
    postOrder(leftChild(tree));
    postOrder(rightChild(tree));
    visitRoot(tree);
}

int main()
{
    BinTree left, right;
    BinTree tree = createBinTree(5);
    left = addToLeft(tree, 4);
    right = addToRight(tree, 3);
    addToLeft(left, 8);
    addToRight(left, 7);
    addToLeft(right, 6);

    printf("先根周游次序：");
    preOrder(tree);
    printf("\n");
    printf("中根周游次序：");
    inOrder(tree);
    printf("\n");
    printf("后根周游算法：");
    postOrder(tree);
    printf("\n");
    return 1;
}



#endif
#if 0
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>

using std::cout;
using std::endl;

char *strstr_wt(const char *src, const char *target)
{
    char *p1 = (char *)src;

    if (!*target)
        return p1;
    while (*p1)
    {
        char *p1begin = p1;
        char *p2 = (char *)target;
        while (*p1 && *p2 && *p1 == *p2)
        {
            p1++;
            p2++;
        }
        if (!*p2)
            return p1begin;
        p1 = p1begin + 1;
    }
    return NULL;
}

bool findchar(char *str, char c)
{
    int i = 0;
    while (!*str)
    {
        if (*str == c)
        {
            return i;
        }
        str++;
        i++;
    }
    return -1;
}

char *strstr_bm(const char *src, const char *target)
{
    char *p1 = (char *)src;
    char *p2 = (char *)target;
    int len = 0, shift = 0, ret = 0;

    if (!*target)
        return p1;

    len = strlen(target);
    while ()
    {
        char *p1 = src+len;
        char *p2 = target+len;
        while (*p1-- == *p2--){}
        
        int bad_pos = target - p2;
        ret = findchar(p2, p1[len-1]);
        if (ret == -1) 
        {
            shift = len;
        }
        else
        {
            shift = ret;
        }
            
        shift = bad_pos - last_pos;
        p1+=shift;
    }
}


int main(int argc, char **argv)
{
    char src[4096] = {0};
    char target[128] = {0};
    char *result = NULL;
    struct timeval tvstart = {0}, tvend = {0};
    long timespend = 0;
    if (argc != 3)
    {
        cout<<"wrong paras: usage: strstr src-string dst-string"<<endl;
        return 0;
    }
    strcpy(src, argv[1]);
    strcpy(target, argv[2]);
    cout<<src<<endl;
    cout<<target<<endl;
    
    gettimeofday( &tvstart, NULL);
    result = strstr_wt(src, target);
    gettimeofday( &tvend, NULL);
    if (result != NULL)
    {
        printf("find the target string: %p/%p\n", src, result);
    }
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"time spend: "<<timespend<<" us"<<endl;
    
    return 0;
}
#endif

#if 0 // strcpy with overlapping area
#include <iostream>
#include <stdlib.h>
#include <string.h>

using std::cout;
using std::endl;

void str_copy(char *src, char *dst, int size)
{
    while (size > 0)
    {
            *dst = *src;
            dst++;
            src++;
            size--;
    }
}

void str_move(char *src, char *dst, int size)
{
    char *shmem = NULL;
    if ((src + size > dst) && (src < dst))
    {
        cout<<"dst and src hava some overlapping area."<<endl;
        shmem = new char[size];
        memset(shmem, 0, size);
        memcpy((void *)shmem, src, size);
        str_copy(shmem, dst, size);
    }
    else
    {
        str_copy(src, dst, size);
    }
    if (shmem)
        delete shmem;
}

int main()
{
    char src[128] = {0};
    char *dst = src + 5;
    char tmp[] = "hello world";

    strncpy(src, tmp, strlen(tmp));
    str_move(src, dst, strlen(src));
    cout<<"move successfully: "<<dst<<endl;

    return 0;
}
#endif

#if 0
int* twoSum(int* nums, int numsSize, int target) 
{
    int *sumindex;
    int n=2*numsSize+1;
    int hash[2][n];
    for(int i=0;i<n;i++)
        hash[1][i]=-1;
    hash[0][nums[0]%n]=nums[0];
    hash[1][nums[0]%n]=0;
    for(int i=1;i<numsSize;i++)
    {
        int k=target-nums[i];
        for(int r=k%n;hash[1][r]!=-1;r=(r+1)%n)
        {
            if(hash[0][r]==k)
            {
                sumindex=(int*)malloc(sizeof(int)*2);
                if(sumindex==NULL)return NULL;
                sumindex[0]=hash[1][r]+1;
                sumindex[1]=i+1;
                return sumindex;
            }
        }
        for(int j=nums[i]%n;;j=(j+1)%n)
            if(hash[1][j]==-1)
            {
                hash[0][j]=nums[i];
                hash[1][j]=i;
                break;
            }
}
#endif

#if 0 //map/set 
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sys/time.h>
#include <map>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::multimap;
using std::map;

template <typename Type> void sort_quick(vector<Type> &, int left, int right);
template <typename Type> int less(Type &, Type &);
template <typename Type> void exch(Type &, Type &);
template <typename Type> void show(vector<Type> &);

map<int, int> twosum(vector<int> &a, int target)
{
    int i = 0; 
    int j = 0; 
    int value = 0; 
    for (i = 0; i < a.size(); i++)
    {
        if (a[i] > target) continue;
        value = target - a[i];
        for (j = i + 1; j < a.size(); j++)
        {
            if (a[j] == value)
            {
                return {{i, a[i]}, {j, a[j]}};
            }
        }
    }
    return map<int, int>();
}

map<int, int> twosum_map(multimap<int, int> &a, int target)
{
    int value = 0;
    for (auto iter = a.begin(); iter != a.end(); iter++)
    {
        if (iter->first > target) 
            break;
        value = target - iter->first;
        auto iter2 = a.find(value);
        while (iter2 != a.end())
        {
            if (iter->second != iter2->second)
            {
                return {{iter->second, iter->first}, {iter2->second, iter2->first}};
            }
            iter2++;
        }
    }
    return map<int, int>();
}

void print_sum(map<int, int> &a, struct timeval tvstart, struct timeval tvend)
{
    long timespend;

    cout<<"---------------------------------------------"<<endl;
    for (auto iter = a.begin(); iter != a.end(); iter++)
    {
        cout<<iter->first<<": "<<iter->second<<endl; 
    }
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"time spend: "<<timespend<<" us"<<endl;
}

int main()
{
    int num = 0, target = 0, i = 0;
    vector<int> array;
    multimap<int, int> imap;
    struct timeval tvstart, tvend;

    cout<<"please input the number you want to process: "<<endl;
    cin>>num;
    while (array.size() < num)
    {
        array.push_back(rand()%10000);
        imap.insert(std::make_pair(array[i], i));
        cout<<array[i++]<<" ";
    }
    cout<<endl;
    cout<<"please input the target: "<<endl;
    cin>>target;

    gettimeofday( &tvstart, NULL);
    auto ret = twosum(array, target);
    gettimeofday( &tvend, NULL);
    print_sum(ret, tvstart, tvend);
    
    gettimeofday( &tvstart, NULL);
    ret = twosum_map(imap, target);
    gettimeofday( &tvend, NULL);
    print_sum(ret, tvstart, tvend);
    return 0;
}
#endif

#if 0
template <typename Type>
void printx(vector<Type> &a)
{
    typename vector<Type>::iterator index = a.begin();
    while (index != a.end())
    {
        cout<<*index<<endl;
        index++;
    }
}

int duplen(string &a, string &b)
{
    int index = 0;
    while (a[index] == b[index])
    {
        index++;
    }

    return index;
}

int findhuge(vector<int> &a)
{
    int index = 0;
    int max = index;
    while (++index < a.size())
    {
        if (a[index] > a[max])
            max = index;
    }
    return max;
}

string dup(string &a)
{
    vector<string> s;
    vector<int> r;
    //vector<string>::iterator vindex;
    string dupstr;
    int index = 0;
    int i = 0;
    int max = 0;

    while (index < a.size())
    {
        s.push_back(a.substr(index));
        index++;
    }
    cout<<"before sort:"<<endl;
    printx(s);
    sort_quick(s, 0, s.size()-1);
    cout<<"after sort:"<<endl;
    printx(s);

    cout<<"s size: "<<s.size()<<endl;

    //find the longest duplicate substring
    for (i = 0; i < s.size()-1; i++)
    {
        r.push_back(duplen(s[i], s[i+1]));
    }
    printx(r);
    max = findhuge(r);
    cout<<"max index: "<<max<<" value: "<<r[max]<<endl;
    cout<<"max duplicate str: "<<s[max].substr(0,r[max])<<endl;
     
    return dupstr;
}

int main(int argc, char **argv)
{
    string a;

    cout<<"please input string: "<<endl;
    cin>>a;
    cout<<a<<" size: "<<a.size()<<endl;

    dup(a);
    return 0;
}
#endif

#if 0 // sort
#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

static int count = 0;

template <class Type> 
int less(Type &a, Type &b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

template <class Type> 
void exch(Type &a, Type &b)
{
    Type tmp = a;
    a = b;
    b = tmp;
    count++;
}

template <class Type> 
void sort_bubble(vector<Type> &a)
{
    int i = 0, j = 0;
    
    for (i = 0; i < a.size() - 1; i++)
    {
        for (j = 0; j < a.size() - i - 1; j++)
        {
            if (a[j] > a[j+1])
            {
                exch(a[j], a[j+1]);
            }
        }
    }
}

template <class Type> 
void sort_select(vector<Type> &a)
{
    int i = 0, j = 0;
    int min = 0;
    
    for (i = 0; i < a.size(); i++)
    {
        min = i;
        for (j = i+1; j < a.size(); j++)
        {
            if (a[j] < a[min])
            {
                min = j;
            }
        }
        if (min != i)
        {
            exch(a[min], a[i]);
        }
    }
}

template <class Type> 
void sort_insert(vector<Type> &a)
{
    int i = 0, j = 0, m = 0;
    int temp = 0;
   
    for (i = 1; i < a.size(); i++)
    {
        for (j = 0; j < i; j++)
        {
            if (a[i] < a[j])
            {
                temp = a[i];
                for (m = i; m > j; m--)
                {
                    a[m] = a[m-1];
                }
                a[j] = temp;
            }
        }
    }
}

template <typename Type>
int partition(vector<Type> &a, int index, int left, int right)
{
    int restore = left;
    Type piovt = a[index];
    int i = 0;

    exch(a[index], a[right]);
    for (i = left; i < right; i++)
    {
        if (a[i] < piovt)
        //if (less(a[i], piovt) < 0)
        {
            if (i != restore)
            {
                exch(a[i], a[restore]);
            }
            restore++;
        }
    }
    exch(a[restore], a[right]);
    return restore;
}

template <typename Type>
void sort_quick(vector<Type> &a, int left, int right)
{
    int index = 0;
    if (right > left)
    {
       index = (right-left+1)/2;
       index = partition(a, left+index, left, right);
       sort_quick(a, left, index - 1);
       sort_quick(a, index + 1, right);
    }
}

template <class Type> 
void show(vector<Type> &a, string b, long time)
{
    int index = 0;
    cout<<"After "<<b<<" sort "<<count<<" changes "<<"time spend: "<<time<<endl;
    while (index < a.size())
    {
        cout<<a[index++]<<"  ";
    }
    cout<<endl;
}

int main(int argc, char **argv)
{
    vector<int> num;
    vector<int> num1;
    vector<int> num2;
    vector<int> num3;
    vector<int> num4;
    unsigned int index = 0;
    unsigned int numinput = 0;
    struct timeval tvstart, tvend;
    long timespend = 0;

    cout<<"Please input the number you want to sort: "<<endl;
    cin>>numinput;
    while (num.size() < numinput)
    {
        num.push_back(rand()%100);
        cout<<num[index++]<<endl;
    }

    show(num, "none", timespend);
    
    num1 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_bubble(num1);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    show(num1, "bubble", timespend);
   
    num2 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_select(num2);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    show(num2, "select", timespend);
    
    num3 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_insert(num3);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    show(num3, "insert", timespend);
  
    num4 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_quick(num4, 0, num4.size()-1);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    show(num4, "quick", timespend);
    return 0;
}
#endif

#if 0 //gcd
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int gcd(int a, int b)
{
   int c = 0;

   c = a % b;
   if (0 == c)
       return b;
   else
       return gcd(b, c);
}

int main(int argc, char **argv)
{
    int a = 0, b = 0;
    cout<<"Input 2 numbers:"<<endl;
    cin>>a;
    cin>>b;
    cout<<"gcd: "<<gcd(a, b)<<endl;
    return 0;
}
#endif
