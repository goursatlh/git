#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <unistd.h>


using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
/* partition function pointer for quick sort */
//template <typename T>

char *rand_str(unsigned int len)
{
#if 0
    char g_arrCharElem[] = {'0', '1', '2', '3','4', '5', '6', '7', '8', '9', 
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
#endif 
    char g_arrCharElem[] = {'a','b','c','d','e','f'}; 
    const int LEN = sizeof(g_arrCharElem); // 26 + 26 + 10 + 2
    char* szStr = new char[len + 1];
    szStr[len] = '\0';
    //srand((unsigned)time(0));
    int iRand = 0;
    for (int i = 0; i < len; ++i)
    {
        iRand = rand() % LEN;            // iRand = 0 - 61
        //cout<<"iRand "<<iRand<<endl;
        szStr[i] = g_arrCharElem[iRand];
    }
    //cout<<"----------------------------------------------------------------"<<endl;
    cout << szStr << endl;
    return szStr;
}

//how to define a global variable in c++ template
template <typename T>
class global_pf {
public:
    typedef int (*pf_func)(vector<T> &vec, int left, int right);
    static pf_func pf_partition;
};

template <typename T>
typename global_pf<T>::pf_func global_pf<T>::pf_partition = NULL; // static variable define

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
void __sort_bubble(vector<T> &vec, int left, int right)
{
    
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
}

template <typename T>
void sort_bubble(vector<T> &vec, int left, int right, const int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    
    gettimeofday(&tvstart, NULL);
    __sort_bubble(vec, left, right);
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
            for (int j = i; j >=left+h && (less(vec[j], vec[j-h]) > 0); j -= h)
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
int partition1(vector<T> &vec, int left, int right)
{
   int guard = vec[left];
   int i = left+1;
   int j = right;
   while(1)
   {
       for (; i <= right; i++)
           if (less(vec[i], guard) < 0)
               break;
       for (; j > left; j--)
           if (less(vec[j], guard) > 0)
               break;
       if (i >= j)
           break;
       exchange(vec[i], vec[j]);
   }
   exchange(vec[left], vec[j]);

   return j;
}

template <typename Type>
int partition2(vector<Type> &a, int left, int right)
{
    int restore = left;
    int index = left + (right-left+1)/2;
    Type piovt = a[index];
    int i = 0;

    exchange(a[index], a[right]);
    for (i = left; i < right; i++)
    {
        if (less(a[i], piovt) > 0)
        {
            if (i != restore)
            {
                exchange(a[i], a[restore]);
            }
            restore++;
        }
    }
    exchange(a[restore], a[right]);
    return restore;
}

template <typename T>
void __sort_quick(vector<T> &vec, int left, int right)
{
    if (!global_pf<T>::pf_partition)
    {
        global_pf<T>::pf_partition = partition2;
    }
    if (right > left)
    {
        int index = global_pf<T>::pf_partition(vec, left, right);
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
    vector<T> vectmp(vec);

    global_pf<T>::pf_partition = partition1;
    gettimeofday(&tvstart, NULL);
    __sort_quick(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by quick sort 1"<<endl;

    global_pf<T>::pf_partition = partition2;
    gettimeofday(&tvstart, NULL);
    __sort_quick(vectmp, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vectmp[find-1]<<" time spend: "<<timespend<<" us by quick sort 2"<<endl;
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
        T max = vec[i];
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

//template <typename T>
vector<int> aux;

template <typename T>
void merge(vector<T> &vec, int left, int mid, int right)
{
#ifdef DEBUG
    cout<<"before merge: "<<left<<" "<<mid<<" "<<right<<endl;
    for (int k = left; k <= right; k++)
    {
        cout<<vec[k]<<" ";
    }
    cout<<endl;
#endif

    int i = left, j = mid+1;
    for (int k = left; k <= right; k++)
        aux[k] = vec[k];

    for (int k = left; k <= right; k++)
    {
        if (i > mid)
            vec[k] = aux[j++];
        else if (j > right)
            vec[k] = aux[i++];
        else if (less(aux[i], aux[j]) < 0)
            vec[k] = aux[j++];
        else
            vec[k] = aux[i++];

    }

#ifdef DEBUG
    //cout<<"after merge: "<<left<<" "<<mid<<" "<<right<<endl;
    cout<<"after merge: "<<endl;
    for (int k = left; k <= right; k++)
    {
        cout<<vec[k]<<" ";
    }
    cout<<endl;
#endif
}

template <typename T>
void __sort_merge(vector<T> &vec, int left, int right)
{
    if (left >= right)
        return;
    int mid = left + (right-left)/2;
    __sort_merge(vec, left, mid);
    __sort_merge(vec, mid+1, right);
    merge(vec, left, mid, right);
}

template <typename T>
void sort_merge(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    aux.resize(right-left+1);
    gettimeofday(&tvstart, NULL);
    __sort_merge(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by merge sort"<<endl;
}

/* pq */
template <typename T>
class MaxPQ {
public:
    MaxPQ(): N(0) 
    { 
        pq.resize(1);
    }
    MaxPQ(int n)
    {
        N = n;
        pq.resize(n+1);
    }
    void insert(T &key);
    T del_max();
    void swim(int k);
    void sink(int k);
    void show();

private:
    vector<T> pq;
    int N;
};

template <typename T>
void MaxPQ<T>::insert(T &key)
{
    pq.push_back(key);
    N += 1;
    swim(N);
}

template <typename T>
T MaxPQ<T>::del_max()
{
    T max = pq[1];
    pq[1] = pq[N];
    N -= 1;
    sink(1);
    return max;
}

template <typename T>
void MaxPQ<T>::swim(int k)
{
    while ((k > 1) && (pq[k] > pq[k/2]))
    {
        exchange(pq[k], pq[k/2]);
        k = k/2;
    }
}

/* algov4 code */
template <typename T>
void MaxPQ<T>::sink(int k)
{
    while (2*k <= N)
    {
        int j = 2*k;
        if (j<N && (less(pq[j], pq[j+1]) < 0))
            j++;
        if (less(pq[k], pq[j]) >= 0)
            break;
        exchange(pq[k], pq[j]);
        k = j;
    }

}

template <typename T>
void __sink(vector<T> &pq, int k, int N)
{
    while (2*k <= N)
    {
        int j = 2*k;
        if (j<N && (less(pq[j], pq[j+1]) > 0))
            j++;
        if (less(pq[k], pq[j]) <= 0)
            break;
        exchange(pq[k], pq[j]);
        k = j;
    }
}

/* my own code */
#if 0
template <typename T>
void MaxPQ<T>::sink(int k)
{
    while (k <= N/2)
    {
        if (2*k+1 <=N)
        {
            if (pq[2*k] > pq[2*k+1])
            {
                if (pq[k] < pq[2*k])
                    exchange(pq[k], pq[2*k]);
                else
                    break;
            }
            else
            {
                if (pq[k] < pq[2*k+1])
                    exchange(pq[k], pq[2*k+1]);
                else
                    break;
            }
        }
        else
        {
            if (pq[k] < pq[2*k])
                exchange(pq[k], pq[2*k]);
            else
                break;
        }
        k = 2*k;
    }
}
#endif

template <typename T>
void MaxPQ<T>::show()
{
    for (int i = 1; i <= N; i++)
        cout<<pq[i]<<" ";
    cout<<endl;
}

template <typename T>
void __sort_heap(vector<T> &vec, int left, int right)
{
    MaxPQ<T> iPQ;
    for (int i = left; i <= right; i++)
    {
        iPQ.insert(vec[i]);
    }
    for (int i = left; i <= right; i++)
    {
        vec[i] = iPQ.del_max();
    }
}

template <typename T>
void __sort_heap_1(vector<T> &vec)
{
    int n = vec.size();
    vec.push_back(vec[0]); // 1 ..... n
    for (int i = n/2; i >= 1; i--)
    {
        __sink(vec, i, n); 
    }
    while (n>1)
    {
        exchange(vec[1], vec[n--]);
        __sink(vec, 1, n);
    }
}

template <typename T>
void sort_heap(vector<T> &vec, int left, int right, int find)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
    vector<T> vectmp(vec);

    gettimeofday(&tvstart, NULL);
    __sort_heap(vec, left, right);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vec[find-1]<<" time spend: "<<timespend<<" us by heap sort 1"<<endl;

    gettimeofday(&tvstart, NULL);
    __sort_heap_1(vectmp);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"sort result is "<<vectmp[find]<<" time spend: "<<timespend<<" us by heap sort 2"<<endl;
}
