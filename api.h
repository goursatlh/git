#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

template <typename T>
void exchange(T &a, T &b)
{
    T tmp = a;
    a = b;
    b =tmp;
}

template <typename T>
int less(const T &a, const T &b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

template <typename T>
void sort_bubble(vector<T> &vec, int left, int right, const int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    
    gettimeofday(&tvstart, NULL);
    for (int i = left; i < right; i++)
    {
        for (int j = left; j < right - i; j++)
        {
            if (vec[j] < vec[j+1])
            {
                exchange(vec[j], vec[j+1]);
            }
        }
    }
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by bubble sort"<<endl;
}

template <typename T>
void __sort_insert(vector<T> &vec, int left, int right)
{
    for (int i = left+1; i <= right; i++)
    {
        for (int j = i; j > left && (less(vec[j], vec[j-1]) > 0); j--)
        {
            exchange(vec[j], vec[j-1]);
        }
    }
}

//#define DEBUG
template <typename T>
void __sort_shell(vector<T> &vec, int left, int right)
{
    int h = 1;
    while (h < (right-left+1)/3)
        h = 3*h+1;
#ifdef DEBUG
    for (auto iter = vec.begin(); iter != vec.end(); iter++)
        cout<<*iter<<" ";
    cout<<endl;
#endif
    while (h >= 1)
    {
        for (int i = left+h; i <= right; i++)
        {
            for (int j = i; j >=h && (less(vec[j], vec[j-h]) > 0); j -= h)
            {
                exchange(vec[j], vec[j-h]);
            }
        }
#ifdef DEBUG
        cout<<"h "<<h<<endl;
        for (auto iter = vec.begin(); iter != vec.end(); iter++)
            cout<<*iter<<" ";
        cout<<endl;
#endif
        h = h/3;
    }
}

template <typename T>
void sort_insert_ex(vector<T> &vec, int left, int right, const int index)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    vector<T> vectmp;
    gettimeofday(&tvstart, NULL);
    for (int i = 0; i < index; i++)
    {
        vectmp.push_back(vec[i]);
    }
    __sort_insert(vectmp, 0, index-1);

    for (int i = index; i <= right; i++)
    {
        if (vec[i] <= vectmp[index-1])
            continue;
        else
        {
            int j;
            //cout<<"this key"<<vec[i]<<" should insert to vector"<<endl;
            for (j = index-2; j >= 0; j-- )
            {
                if (vec[i] > vectmp[j])
                    continue;
                else
                {
                    //cout<<vec[i]<<" should insert behind index "<<j<<endl;
                    for (int k = index-1; k > j+1; k--)
                    {
                        vectmp[k] = vectmp[k-1];
                    }
                    vectmp[j+1] = vec[i];
                    break;
                }
            }
            if (j < 0)
            {
                for (int k = index-1; k > 0; k--)
                {
                    vectmp[k] = vectmp[k-1];
                }
                vectmp[0] = vec[i];
            }
        }
    }
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vectmp[index-1]<<" time spend: "<<timespend<<" us by insert_ex sort"<<endl;
}

// 0 1 2 3 4 5
// 5 9 2 7 3 4
template <typename T>
int partition(vector<T> &vec, int left, int right)
{
   int guard = vec[left];
   int i = left+1;
   int j = right;
   while(1)
   {
       for (; i <= right; i++)
           //if (vec[i] < guard)
           if (less(vec[i], guard) < 0)
               break;
       for (; j > left; j--)
           //if (vec[j] > guard)
           if (less(vec[j], guard) > 0)
               break;
       if (i >= j)
           break;
       exchange(vec[i], vec[j]);
   }
   exchange(vec[left], vec[j]);

   return j;
}

template <typename T>
void __sort_quick(vector<T> &vec, int left, int right)
{
    if (right > left)
    {
        int index = partition(vec, left, right);
        __sort_quick(vec, left, index-1);
        __sort_quick(vec, index+1, right);
    }
    return;
}

template <typename T>
void sort_quick(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    gettimeofday(&tvstart, NULL);
    __sort_quick(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by quick sort"<<endl;

}

template <typename T>
void sort_insert(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    gettimeofday(&tvstart, NULL);
    __sort_insert(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by insert sort"<<endl;

}

template <typename T>
void sort_shell(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    gettimeofday(&tvstart, NULL);
    __sort_shell(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by shell sort"<<endl;

}

template <typename T>
void __sort_choose(vector<T> &vec, int left, int right)
{
    for (int i = left; i <= right; i++)
    {
        int max = vec[i];
        int index = i;
        for (int j = i+1; j <= right; j++)
        {
            if (vec[j] > max)
            {
                max = vec[j];
                index = j;
            }
        }
        if (i != index)
            exchange(vec[i], vec[index]);
    }
}

template <typename T>
void sort_choose(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    gettimeofday(&tvstart, NULL);
    __sort_choose(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by choose sort"<<endl;
}
