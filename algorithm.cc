
#if 1 // sort
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

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
}

template <class Type> 
void sort_bubble(vector<Type> &a, int *change)
{
    int i = 0, j = 0;
    int chag = 0;
    
    for (i = 0; i < a.size() - 1; i++)
    {
        for (j = 0; j < a.size() - i - 1; j++)
        {
            if (a[j] > a[j+1])
            {
                exch(a[j], a[j+1]);
                chag++;
            }
        }
    }
    *change = chag;
}

template <class Type> 
void sort_select(vector<Type> &a, int *change)
{
    int i = 0, j = 0;
    int min = 0;
    int compare = 0;
    int chag = 0;
    
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
            chag++;
        }
    }
    *change = chag;
}

template <class Type> 
void show(vector<Type> &a, int change, string b)
{
    int index = 0;
    cout<<"After "<<b<<" sort "<<change<<" changes:"<<endl;
    while (index < a.size())
    {
        cout<<a[index++]<<"  ";
    }
    cout<<endl;
}

int main(int argc, char **argv)
{
    vector<int> num;
    int change = 0;
    unsigned int count = 0;
    unsigned int index = 0;
    cout<<"Please input the number you want to sort: "<<endl;
    cin>>count;
    while (num.size() < count)
    {
        num.push_back(rand()%100);
        cout<<num[index++]<<endl;
    }
    sort_bubble(num, &change);
    show(num, change, "bubble");
    sort_select(num, &change);
    show(num, change, "select");
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
