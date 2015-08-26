#if 1 //suffix array
#include <iostream>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main(int argc, char **argv)
{

    return 0;
}
#endif

#if 0 // sort
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

template <typename Type> void sort_select(vector<Type> &);
//template <typename Type> int less(Type &, Type &);
//template <typename Type> bool is_sorted(vetcor<Type> &);
template <typename Type> void exch(Type &, Type &);
template <typename Type> void show(vector<Type> &);

static int count = 0;

template <class Type> 
int less(Type a, Type b)
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
    int piovt = a[index];
    int i = 0;

    exch(a[index], a[right]);
    for (i = left; i < right; i++)
    {
        if (a[i] < piovt)
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

#if 0
template <typename Type>
int partition(vector<Type> &a, int index, int left, int right)
{
    int poivt = a[index];
    int position = left;
    int i = 0;

    exch(a[index], a[right]);
    for (i = left; i < right; i++)
    {
        if (a[i] <= poivt)
        {
            exch(a[i], a[position]);
            position++;
        }
    }
   
    exch(a[position], a[right]);
    return position;
}

template <class Type> 
void sort_quick(vector<Type> &a, int left, int right)
{
    int index = 0;
    if (right > left)
    {
        index = (right - left + 1)/2;
        index = partition(a, left+index, left, right);
        sort_quick(a, left, index - 1);
        sort_quick(a, index + 1, right);
    }
}
#endif

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
