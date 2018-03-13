
#if 1  // max sum for the sub-list


#endif
// bad example for recursion
#if 0
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

int fib(int n)
{
    if (n <= 1)
        return 1;
    else
        return fib(n-1)+fib(n-2);
}

int main()
{
    int ret = 0;
    int num = 0;
    cout<<"please input the number: "<<endl;
    cin>>num;

    ret = fib(num);
    cout<<"result is "<<ret<<endl;
    return 0;
}
#endif

#if 0 // minimum spanning tree
class EdgeWeightedGraph
{
public:
    EdgeWeightedGraph(int v) {}
private:

};

int main()
{
    return 0;
}
#endif

#if 0 /* priority queue */ 

#include <vector>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

class MaxPQ
{
public:
    MaxPQ(vector<int> &a)
    {
        pq.push_back(0); // not use index 0
        N = 0;
        for (auto iter = a.begin(); iter != a.end(); iter++)
        {
            insert(*iter);
        }
    }

    void insert(int key)
    {
        //cout<<"insert key "<<key<<endl;
        pq.push_back(key);
        ++N; 
        swim(N);
    }

    int max()
    {
        cout<<"Max vaule "<<pq[1]<<endl;
        return pq[1];
    }

    int delMax()
    {
        if (N > 1)
        {
            vector<int> pqtmp;
            pqtmp = pq;
           
            cout<<"del max number "<<pq[1]<<endl;
            vector<int>::iterator iter = pqtmp.begin();
            iter++; // thift the index 0
            iter++; // thift max value

            // rebuild the heap array
            N = 0; 
            pq.clear();
            pq.push_back(0);
            for (; iter != pqtmp.end(); iter++)
            {
                insert(*iter);
            }
        }
        else
            cout<<"del max number "<<pq[1]<<endl;
    }

    int size()
    {
        return N;        
    }

    void show()
    {
        cout<<"Total number "<<N<<endl;
        vector<int>::iterator iter = pq.begin();
        for (++iter; iter != pq.end(); iter++)
        {
            cout<<*iter<<" ";
        }
        cout<<endl;
    }

private:
    vector<int> pq; // vector to store the elements of the priority queue.
    int N;          // number of elements in the priority queue. 
    
    void swim(int k)
    {
        //cout<<"swim "<<k<<endl;
        while ((k >1) && less(k/2, k)) 
        {
            exch(k/2, k);
            k = k/2;
        }
    }
    
    void sink(int k)
    {
        cout<<"sink "<<k<<endl;
        while (2*k < N)
        {
            if (less(k, 2*k) && !less(k, 2*k+1))
            {
                exch(k, 2*k);
            }
            else if (less(k, 2*k+1) && !less(k, 2*k))
            {
                exch(k, 2*k+1);
            }
            else if (less(k, 2*k+1) && less(k, 2*k))
            {
                if (less(2*k, 2*k+1))
                    exch(k, 2*k+1);
                else
                    exch(k, 2*k);
            }
            k = 2*k;
        }
    }

    bool less(int i, int j)
    {
        if (pq[i] < pq[j])
            return 1;
        else
            return 0;
    }

    void exch(int i, int j)
    {
        //cout<<"exch "<<i<<" and "<<j<<endl;
        int tmp = pq[i];
        pq[i] = pq[j];
        pq[j] = tmp;
    }
};

int main()
{
    vector<int> a;
    int num = 0;
    cout<<"Input number you want to process: "<<endl;
    cin>>num;
    for (int i = 0; i < num; i++)
    {
        a.push_back(rand()%100);
    }

    for (auto iter = a.begin(); iter != a.end(); iter++)
    {
        cout<<*iter<<" "; 
    }
    cout<<endl;

    MaxPQ pq(a);
    pq.show();
    pq.max();

    cout<<"max number "<<pq.size()<<endl;
    num = pq.size();
    for (int i = 0; i < num; i++)
    {
        pq.delMax();
    }
    return 0;
}





#endif /* priority queue */


#if 0 // SPF algo
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class DirectedEdge
{
private:
    int v = 0;
    int w = 0;
    double weight = 0.0;

public:
    DirectedEdge(int v, int w, double weight)
    {
        this->v = v;
        this->w = w;
        this->weight = weight;
    }
    DirectedEdge() = default;

    double getWeight()
    {
        return weight;
    }
    int from()
    {
        return v;
    }
    int to()
    {
        return w;
    }
    string toString()
    {
        string str;
        char a[32];
        sprintf(a, "%d->%d %.2f", v, w, weight);
        str = a;
        return str;
    }
};

class EdgeDirectedDigraph
{
    int V;
    int E;
    vector<DirectedEdge> adj; 
   
public:
    EdgeDirectedDigraph(int V)
    {
        V = V;
        E = 0;
        
    }
    void addEdge(V)
    {
        E++;
    }
    double weight();
    int from();
    int to();
    string toString;
};

int main()
{
    cout<<"hello world"<<endl;
    DirectedEdge e1(1, 2, 3);
    DirectedEdge e2;
    cout<<e1.toString()<<endl;
    cout<<e2.toString()<<endl;
    return 0;
}


#endif
#if 0 // stability for sort algorithum
#include "api.h"

class Student 
{
private:
    int no;
    int score;
public:
    Student(const int no, int score): no(no), score(score) {}
    Student() = default;
    bool operator>(const Student &a) const {return score > a.score;}
    bool operator<(const Student &a) const {return score < a.score;}
    bool operator==(const Student &a) const {return score > a.score;}
    void show()
    {
        cout<<no<<":"<<score<<" ";
    }
};

int main()
{
    vector<Student> vecstu;
    int num = 0;
    cout<<"Input number you want to process: "<<endl;
    cin>>num;
    for (int i = 0; i < num; i++)
    {
        vecstu.push_back(Student(rand()%100, rand()%10));
    }
    vector<Student> vecstu2(vecstu);
    vector<Student> vecstu3(vecstu);
    vector<Student> vecstu4(vecstu);

    cout<<"before sort:"<<endl;
    for (auto iter = vecstu.begin(); iter != vecstu.end(); iter++)
    {
        iter->show();
    }
    cout<<endl;
    __sort_insert<Student>(vecstu, 0, vecstu.size()-1);
    for (auto iter = vecstu.begin(); iter != vecstu.end(); iter++)
    {
        iter->show();
    }
    cout<<" insert sort"<<endl;

    __sort_bubble<Student>(vecstu3, 0, vecstu3.size()-1);
    for (auto iter = vecstu3.begin(); iter != vecstu3.end(); iter++)
    {
        iter->show();
    }
    cout<<" bubble sort"<<endl;

    __sort_choose<Student>(vecstu2, 0, vecstu2.size()-1);
    for (auto iter = vecstu2.begin(); iter != vecstu2.end(); iter++)
    {
        iter->show();
    }
    cout<<" choose sort"<<endl;

    __sort_heap<Student>(vecstu4, 0, vecstu4.size()-1);
    for (auto iter = vecstu4.begin(); iter != vecstu4.end(); iter++)
    {
        iter->show();
    }
    cout<<" heap sort"<<endl;

    return 0;
}
#endif

#if 0
#include "api.h"

int main()
{
    char a[] = {'A', 'E', 'I', 'O', 'P', 'R', 'S', 'T'};
    MaxPQ<char> cPQ;
    for (int i = 0; i < sizeof(a); i++)
        cPQ.insert(a[i]);
    cPQ.show();
    cPQ.del_max();
    cPQ.show();
    return 0;
}
#endif

#if 0//find the kst number from N numbers and for quick sort research
#include "api.h"
#include <thread>
#include <time.h>
#include <sys/time.h>

using std::thread;

int main()
{
    unsigned int total = 0;
    unsigned int find = 0;
    vector<int> vecInt;
    vector<thread> threads;
    cout<<"Please input the total number and which number you want to find: "<<endl;
    cin>>total>>find;
    if (find > total)
    {
        cout<<"bad input: find can't be great than total"<<endl;
        return 0;
    }
    cout<<"total num: "<<total<<" "<<find<<endl;
    srand((unsigned)time(0));
    for (int i = 0; i < total; i++)
    {
        //vecInt.push_back(rand()%100);   
        vecInt.push_back(rand()%1000000);   
    }

    vector<int> vecInt2(vecInt);
    vector<int> vecInt3(vecInt);
    vector<int> vecInt4(vecInt);
    vector<int> vecInt5(vecInt);
    vector<int> vecInt6(vecInt);
    vector<int> vecInt7(vecInt);
    vector<int> vecInt8(vecInt);

    threads.push_back(thread(sort_bubble<int>, std::ref(vecInt),  0, (vecInt.size()-1), find));
    threads.push_back(thread(sort_insert<int>, std::ref(vecInt2), 0, (vecInt2.size()-1), find));
    //threads.push_back(thread(sort_insert_ex<int>, std::ref(vecInt3), 0, (vecInt3.size()-1), find));
    threads.push_back(thread(sort_quick<int>,  std::ref(vecInt4), 0, (vecInt4.size()-1), find));
    threads.push_back(thread(sort_choose<int>,  std::ref(vecInt5), 0, (vecInt5.size()-1), find));
    threads.push_back(thread(sort_shell<int>,  std::ref(vecInt6), 0, (vecInt6.size()-1), find));
    threads.push_back(thread(sort_merge<int>,  std::ref(vecInt7), 0, (vecInt7.size()-1), find));
    threads.push_back(thread(sort_heap<int>,  std::ref(vecInt8), 0, (vecInt8.size()-1), find));
   
    for (auto& t: threads) 
    {
        t.join();
    }
    return 0;
}
#endif

#if 0
#include <iostream>
#include <vector>
using std::vector;

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
        if (less(a[i], piovt) < 0)
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
#endif

#if 0 //suffix array
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

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
        if (less(a[i], piovt) < 0)
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

class SuffixArray
{
    //string *suffixes;
    vector<string> suffixes;
    unsigned int len;
public:
    SuffixArray(string &str)
    {
       len = str.size();
       //suffixes = new string[len];
       for (int i = 0; i < len; i++)
       {
           //suffixes[i] = str.substr(i);
           suffixes.push_back(str.substr(i));
           //cout<<suffixes[i]<<endl;
       }
       sort_quick(suffixes, 0, len-1);
    }
    void print();
};

void SuffixArray::print()
{
    //string *p = suffixes;
    for (int i = 0; i < len; ++i)
    {
        cout<<suffixes[i]<<endl;
    }
}

int main()
{
    string str;
    cout<<"please input the string: "<<endl;
    cin>>str;
    SuffixArray sa(str);
    sa.print();
    return 0;
}
#endif

#if 0 //find the longest same substring => suffix array
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <string>
#include "api.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

/* find the longest same prefix of s1 and s2 */
unsigned int lsd_prefix(string &s1, string &s2)
{
    unsigned int len = s1.size();
    if (s1.size() > s2.size())
    {
        len = s2.size();
    }
    for (int i = 0; i < len; i++)
    {
        if (s1[i] != s2[i])
        {
            return i;
        }
    }
    return len;
}

/* find the longest same substring of s1 and s2 */
void lsd(string &s1, string &s2)
{
    unsigned int len = 0;
    vector<unsigned int> index;
    unsigned int max_len = len;
    for (int i = 0; i < s1.size(); i++)
    {
        for (int j = 0; j < s2.size(); j++)
        {
           string ss1(s1.substr(i));
           string ss2(s2.substr(j));
           len = lsd_prefix(ss1, ss2);
           if (len > max_len)
           {
               index.clear();
               index.push_back(i);
               max_len = len;
           }
           else if ((len == max_len) && (len > 0))
           {
               index.push_back(i);
           }
        }
    }

    for (auto iter = index.begin(); iter != index.end(); iter++)
    {
        cout<<"lsd: "<<s1.substr(*iter, max_len)<<endl;
    }
}

/* find the longest dupulicat substring in on string */
void ldss(string &s1)
{
    unsigned int len = 0;
    unsigned int max_len = len;
    vector<unsigned int> index;
    vector<string> suffix;

    for (int i = 0; i < s1.size(); i++)
    {
        suffix.push_back(s1.substr(i));
    }
    
    __sort_quick<string>(suffix, 0, (suffix.size()-1));
   
#if 1
    for (auto iter = suffix.begin(); iter != suffix.end(); iter++)
    {
        cout<<*iter<<endl;
    }
#endif
    for (int j = 0; j < suffix.size()-1; j++)
    {
       len = lsd_prefix(suffix[j], suffix[j+1]);
       if (len > max_len)
       {
           index.clear();
           index.push_back(j);
           max_len = len;
       }
       else if ((len == max_len) && (len > 0))
       {
           index.push_back(j);
       }
    }

    for (auto iter1 = index.begin(); iter1 != index.end(); iter1++)
    {
        //cout<<"lsd: index "<<*iter1<<" "<<suffix[*iter1].substr(0, max_len)<<endl;
        cout<<"ldss: "<<suffix[*iter1].substr(0, max_len)<<endl;
    }
}

#if 0 // lsd for two strings
int main(int argc, char **argv)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
#if 1
    if (argc != 3)
    {
        cout<<"please input right paras: xx len1 len2"<<endl;
        return -1;
    }
    srand((unsigned)time(0));
    char *s1 = rand_str(atoi(argv[1]));
    char *s2 = rand_str(atoi(argv[2]));
    string str1(s1);
    string str2(s2);
    if (s1)
        delete [] s1;
    if (s2)
        delete [] s2;
#endif

#if 0
    string str1;
    string str2;
    cin>>str1>>str2;
#endif
    gettimeofday(&tvstart, NULL);
    lsd(str1, str2);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"time spend: "<<timespend<<" usec"<<endl; 
    return 0;
}
#endif

#if 1
int main(int argc, char **argv)
{
    struct timeval tvstart, tvend;
    long timespend = 0;
#if 0
    if (argc != 2)
    {
        cout<<"please input right paras: xx len1 len2"<<endl;
        return -1;
    }
    srand((unsigned)time(0));
    char *s1 = rand_str(atoi(argv[1]));
    string str1(s1);
    if (s1)
        delete [] s1;
#endif

#if 1
    string str1;
    //cin>>str1;
    getline(cin, str1);
#endif
    gettimeofday(&tvstart, NULL);
    ldss(str1);
    gettimeofday(&tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    cout<<"time spend: "<<timespend<<" usec"<<endl; 
    return 0;
}
#endif
#endif

#if 0 // hash_map
#include <iostream>
#include <string.h>
#include <ext/hash_map>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using namespace __gnu_cxx;

struct hash_string {
size_t operator()(const string &str) const
{
    unsigned int h = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        h = h*5 + str[i];
    }
    return (size_t)h;
}
};

struct compare_string {
bool operator()(const string &str1, const string &str2) const
{
    cout<<"compare func"<<endl;
    return str1 == str2;
}
};

int main(void)
{
//hash_map<int, string> my_map;
hash_map<string, int, hash_string, compare_string> my_map;
unsigned int num = 0;
cout<<"Please input the num you want to hash: "<<endl;
do 
{
    cin>>num;
}while (num <= 0);

string b;
for (int i = 0; i < num; i++)
{
    b.clear();
    unsigned int len = 1 + rand()%10;
    for (int j = 0; j < len; j++)
    {
        b.push_back('a'+rand()%26);
    }
    cout<<i<<" "<<b<<endl;
    my_map[b] = rand()%100;
}

if (my_map.find(b) != my_map.end())
{
    cout<<"find the key: "<<b<<endl;
}
else
{
    cout<<"can't find the key"<<endl;
}
cout<<sizeof(my_map)<<endl;
cout<<my_map.size()<<endl;
return 0;
}
#endif

#if 0 // rb tree
#include <iostream>
#include <list>

using std::cout;
using std::cin;
using std::endl;
using std::list;

class Node
{
public:
Node *left;
Node *right;
int color;
#define RED 0
#define BLACK 1
int key;

Node(int key, int color): key(key), color(color) 
{
    left = right = NULL;
}
};

class RBT
{
Node *root;
public:
RBT(): root(NULL) {}
//public api
void put(int key);  
void get(int key);  
void del(int key);  
void print();  
void NatureDisplayTree();
private:
//process balance
void rotate_left(Node *&x);
void rotate_right(Node *&x);
bool is_red(Node *x);
void flip_color(Node *x);

//internal api
int get(Node *x, int key);
void put(Node *&x, int key);
void print(Node *x);  
};

class DisplayInfo  
{  
public:  
int level;  
int pos;        //结点在屏幕中的绝对位置  
bool enter;  
int spaceNum;  
}; 

void RBT::NatureDisplayTree()
{
int i;
list<Node *>Q;
list<DisplayInfo>QI;
int screenWidth=64;
int dataWidth=2;
DisplayInfo info;    //将插入队列的结点的打印信息
DisplayInfo preInfo; //队尾的结点的打印信息
Node *curNode;       //队列当前取出的结点
DisplayInfo curInfo; //队列当前取出的结点的打印信息
if(!root)
{
    printf("Tree is empty !\n");
    return;
}

printf("Nature Display Tree:\n");
Q.push_back(root);
info.level=1;
info.enter=true;
info.spaceNum=screenWidth>>info.level;
info.pos=info.spaceNum;
QI.push_back(info);
preInfo=info;
while(Q.size())
{
    curNode=Q.front();
    Q.pop_front();
    curInfo=QI.front();
    if(curInfo.enter) 
        printf("\n\n");
    for (i=0;i<curInfo.spaceNum;i++)
        printf(" ");
    printf("%2d(%d)",curNode->key, curNode->color);
    QI.pop_front();
    if(curNode->left)
    {
        Q.push_back(curNode->left);
        info.level=curInfo.level+1;
        info.pos=curInfo.pos-(screenWidth>>info.level);
        if(info.level>preInfo.level)
        {
            info.enter=true;
            info.spaceNum=info.pos;
        }
        else
        {
            info.enter=false;
            info.spaceNum=info.pos-preInfo.pos;
        }
        info.spaceNum-=dataWidth;
        QI.push_back(info);
        preInfo=info;

    }
    if(curNode->right)
    {
        Q.push_back(curNode->right);
        info.level=curInfo.level+1;
        info.pos=curInfo.pos+(screenWidth>>info.level);
        if(info.level>preInfo.level)
        {
            info.enter=true;
            info.spaceNum=info.pos;
        }
        else
        {
            info.enter=false;
            info.spaceNum=info.pos-preInfo.pos;
        }
        info.spaceNum-=dataWidth;
        QI.push_back(info);
        preInfo=info;
    }

}
printf("\n");
}

void RBT::print()
{
print(root);
}

void RBT::print(Node *x)
{
if (x)
{
    print(x->left);
    cout<<x->key<<endl;
    print(x->right);
}
}

void RBT::put(Node *&x, int key)
{
Node *p;
if (x == NULL)
{
   x = new Node(key, RED);
   return;
}
if (key < x->key)
   put(x->left, key);
else if (key > x->key)
   put(x->right, key);
else
   cout<<"duplicate key, can't insert to tree"<<endl;

if (is_red(x->right) && !is_red(x->left))
    rotate_left(x);
if (is_red(x->left) && is_red(x->left->left))
    rotate_right(x);
if (is_red(x->left) && is_red(x->right))
    flip_color(x);
}

void RBT::put(int key)
{
put(root, key);
if (root)
    root->color = BLACK;
}

void RBT::rotate_left(Node *&x)
{
Node * t = x->right;
x->right = t->left;
t->left = x;
t->color = x->color;
x->color = RED;
x = t;
}

void RBT::rotate_right(Node *&x)
{
Node * t = x->left;
x->left = t->right;
t->right = x;
t->color = x->color;
x->color = RED;
x = t;
}

void RBT::flip_color(Node *x)
{
x->color = RED;
x->left->color = BLACK;
x->right->color = BLACK;
}

bool RBT::is_red(Node *x)
{
if (x == NULL)
    return false;
return (x->color == RED);
}

int main(int argc, char **argv)
{
RBT t;
int num = 0;
int a = 0;
// test the insert operation
cout<<"please input the number you want to play: "<<endl;
do {
    cin>>num;
} while (num <= 0);

while (num--)
{
    //t.put(rand()%100);
    t.put(num);
}
t.print();
t.NatureDisplayTree();
#if 0 
// test the delete operation
cout<<"plese input the key you want to delete: "<<endl;
if (cin>>a)
{
    if (!t.get(a))
        t.del(a);
    else
        cout<<"the key you input doesn't exist"<<endl;
}

t.print();
t.NatureDisplayTree();
#endif
return 0;
}
#endif

#if 0 // binary tree/red-black tree
#include <iostream>
#include <list>

using std::cout;
using std::cin;
using std::endl;
using std::list;

class DisplayInfo  
{  
public:  
int level;  
int pos;        //结点在屏幕中的绝对位置  
bool enter;  
int spaceNum;  
}; 

class Node
{
public:
Node *left;
Node *right;
int key;
//int num; /\num of nodes of the tree which use this node as the root.
Node(int key, Node *left, Node *right): 
     key(key), left(left), right(right) {}
};

class BST 
{
Node *root;
//how to present the node(not root node)
public:
BST(): root(NULL) {}
int get(int key); // how to insert a new node and make the tree ordered and balance.
void put(int key);   
void del(int key);
void del_min();
void del_min(Node *&x);
void print();
void NatureDisplayTree();  
private:
int get(Node *x, int key);
void put(Node *&n, int key);
void del(Node *&x, int key);
Node *min(Node *x);
void print(Node *n);
};

int BST::get(Node *x, int key)
{
if (x == 0)
    return -1;
if (key < x->key)
    get(x->left, key);
else if (key > x->key)
    get(x->right, key);
else
    return 0;
}

int BST::get(int key)
{
return get(root, key);
}

void BST::put(Node *&x, int key)
{
if (x == NULL)
{
   x = new Node(key, NULL, NULL);
   return;
}
if (key < x->key)
   put(x->left, key);
else if (key > x->key)
   put(x->right, key);
else
   cout<<"duplicate key, can't insert to tree"<<endl;
}

void BST::put(int key)
{
return put(root, key);
}

Node *BST::min(Node *x)
{
if (x == NULL)
    return NULL;
else if (x->left == NULL)
    return x;
else 
    return min(x->left);
}
void BST::del(int key)
{
del(root, key);
}

void BST::del(Node *&x, int key)
{
if (x->key == key)
{
    if (x->left == NULL)
        x = x->right;
    else
    {
        if (x->right)
        {
            Node *p = min(x->right);
            del_min(x->right);
            p->left = x->left;
            p->right = x->right;
            x  = p;
        }
        else
            x = x->left;
    }
}
else if (key < x->key)
    del(x->left, key);
else if (key > x->key)
    del(x->right, key);
}

void BST::del_min()
{
del_min(root);
}

void BST::del_min(Node *&x)
{
if (x->left == NULL)
    //delete this node
    if (x->right == NULL)
        x = NULL;
    else
        x = x->right;
else
    del_min(x->left);
}

void BST::print()
{
cout<<"trees: "<<endl;
print(root); 
}

void BST::print(Node *x)
{
if (x)
{
    print(x->left);
    cout<<x->key<<endl;
    print(x->right);
}
}

void BST::NatureDisplayTree()
{
int i;
list<Node *>Q;
list<DisplayInfo>QI;
int screenWidth=64;
int dataWidth=2;
DisplayInfo info;    //将插入队列的结点的打印信息
DisplayInfo preInfo; //队尾的结点的打印信息
Node *curNode;       //队列当前取出的结点
DisplayInfo curInfo; //队列当前取出的结点的打印信息
if(!root)
{
    printf("Tree is empty !\n");
    return;
}

printf("Nature Display Tree:\n");
Q.push_back(root);
info.level=1;
info.enter=true;
info.spaceNum=screenWidth>>info.level;
info.pos=info.spaceNum;
QI.push_back(info);
preInfo=info;
while(Q.size())
{
    curNode=Q.front();
    Q.pop_front();
    curInfo=QI.front();
    if(curInfo.enter) 
        printf("\n\n");
    for (i=0;i<curInfo.spaceNum;i++)
        printf(" ");
    printf("%2d",curNode->key);
    QI.pop_front();
    if(curNode->left)
    {
        Q.push_back(curNode->left);
        info.level=curInfo.level+1;
        info.pos=curInfo.pos-(screenWidth>>info.level);
        if(info.level>preInfo.level)
        {
            info.enter=true;
            info.spaceNum=info.pos;
        }
        else
        {
            info.enter=false;
            info.spaceNum=info.pos-preInfo.pos;
        }
        info.spaceNum-=dataWidth;
        QI.push_back(info);
        preInfo=info;

    }
    if(curNode->right)
    {
        Q.push_back(curNode->right);
        info.level=curInfo.level+1;
        info.pos=curInfo.pos+(screenWidth>>info.level);
        if(info.level>preInfo.level)
        {
            info.enter=true;
            info.spaceNum=info.pos;
        }
        else
        {
            info.enter=false;
            info.spaceNum=info.pos-preInfo.pos;
        }
        info.spaceNum-=dataWidth;
        QI.push_back(info);
        preInfo=info;
    }

}
printf("\n");
}

int main(int argc, char **argv)
{
BST t;
int num = 0;
int a = 0;
cout<<"please input the number you want to play: "<<endl;
cin>>num;
while (num--)
{
    t.put(rand()%100);
}
t.print();
t.NatureDisplayTree();
cout<<"delete the min node."<<endl;
t.del_min();
t.print();
t.NatureDisplayTree();
cout<<"plese input the key you want to delete: "<<endl;
if (cin>>a)
{
    if (!t.get(a))
        t.del(a);
    else
        cout<<"the number you input is not existed"<<endl;
}
t.print();
t.NatureDisplayTree();
return 0;
}
#endif

#if 0 //greedy algorithum
#include <iostream>  
using namespace std;  

void GreedyChoose(int len,int *s,int *f,bool *flag);  

int main(int argc, char* argv[])  
{  
int s[11] ={1,3,0,5,3,5,6, 8, 8, 2, 12};  
int f[11] ={4,5,6,7,8,9,10,11,12,13,14};  

bool mark[11] = {0};  

GreedyChoose(11,s,f,mark);  
for(int i=0;i<11;i++)  
    if(mark[i])  
        cout<<i<<" ";  
//system("pause");  
return 0;  
}  

void GreedyChoose(int len,int *s,int *f,bool *flag)  
{  
flag[0] = true;  
int j = 0;  
for(int i=1;i<len;++i)  
    if(s[i] >= f[j])  
    {  
        flag[i] = true;  
        j = i;  
    }  
}  
#endif

#if 0 //string sort1: key-index sort
#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

struct timeval tvstart, tvend;

void show(vector<string> &a)
{
long timespend = 0;
for (int i = 0; i < a.size(); i++)
{
    cout<<a[i]<<endl;
}
timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
cout<<"time spend: "<<timespend<<endl;
}

int R = 256;         // radix 
int M = 5;          // cutoff for small subarrays 
int charAt(string s, int d) 
{  
if (d < s.length()) 
    return s.at(d); 
else 
    return -1;               
} 

void sort(vector<string> a, int lo, int hi, int d) 
{    
vector<string> aux;  // auxiliary array for distribution 

if (hi <= lo + M) 
{  Insertion.sort(a, lo, hi, d); return;  } 

int[] count = new int[R+2];               // Compute frequency counts. 
for (int i = lo; i <= hi; i++) 
    count[charAt(a[i], d) + 2]++; 

for (int r = 0; r < R+1; r++)             // Transform counts to indices. 
    count[r+1] += count[r]; 

for (int i = lo; i <= hi; i++)            // Distribute. 
    aux[count[charAt(a[i], d) + 1]++] = a[i]; 

for (int i = lo; i <= hi; i++)            // Copy back. 
    a[i] = aux[i - lo]; 

// Recursively sort for each character value. 
for (int r = 0; r < R; r++) 
    sort(a, lo + count[r], lo + count[r+1] - 1, d+1); 

} 

void sort_msd(vector<string> &a)
{
int N = a.size(); 
sort(a, 0, N-1, 0); 
}

void sort_lsd(vector<string> &a, int W) 
{   // Sort a[] on leading W characters. 
int N = a.size(); 
int R = 256; 
vector<string> aux(a);

for (int d = W-1; d >= 0; d--) 
{ // Sort by key-indexed counting on dth char. 

    vector<int> count(R+1, 0);                // Compute frequency counts.
    cout<<"count"<<endl;
    for (int i = 0; i < N; i++)
    {
        count[a[i].at(d) + 1]++;
        cout<<"index "<<a[i].at(d)+1<<" "<<count[a[i].at(d) + 1]<<endl;
    }
    cout<<"step2"<<endl;
    for (int r = 0; r < R; r++)               // Transform counts to indices.
    {
        count[r+1] += count[r];
        cout<<"index "<<r<<"==> "<<count[r]<<endl;

    }
    for (int i = 0; i < N; i++)               // Distribute. 
        aux[count[a[i].at(d)]++] = a[i]; 

    for (int i = 0; i < N; i++)               // Copy back. 
        a[i] = aux[i]; 
} 
} 

int main()
{
vector<string> a;
string b;
int num, len;
cout<<"Please input the length and the number of the string you want to sort: "<<endl;
cin>>len;
cin>>num;

//construct the array
for (int i = 0; i < num; i++)
{
    b.clear();
    for (int j = 0; j < len; j++)
    {
        b.push_back('a'+rand()%26);
    }
    a.push_back(b);
}
//do the string sort
cout<<"before sort: "<<endl;
show(a);
cout<<"after sort: "<<endl;
gettimeofday(&tvstart, NULL);
sort_lsd(a, len);
gettimeofday(&tvend, NULL);
show(a);

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

#if 0 // sort algorithum
#include <iostream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <algorithm>

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
#if 0
    while (index < a.size())
    {
        cout<<a[index++]<<"  ";
    }
    cout<<endl;
#endif
}

template <class Type> 
void check_sort_result(vector<Type> &a, vector<Type> &b, struct timeval tvstart, struct timeval tvend, string type)
{
    long timespend = 0;
    if (a == b)
    {
        timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
        show(a, type, timespend);
    }
    else
        cout<<type<<" sort failed."<<endl;
}

int main(int argc, char **argv)
{
    vector<int> num;
    vector<int> num_sorted;
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
        num.push_back(rand() % 100000);
    }
    show(num, "origin", timespend);
    
    num_sorted = num;
    gettimeofday( &tvstart, NULL);
    sort(num_sorted.begin(), num_sorted.end());
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    show(num_sorted, "built-in sort", timespend);
    
    num1 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_bubble(num1);
    gettimeofday( &tvend, NULL);
    check_sort_result(num1, num_sorted, tvstart, tvend, "bubble");

    num2 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_select(num2);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    check_sort_result(num2, num_sorted, tvstart, tvend, "select");
    
    num3 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_insert(num3);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    check_sort_result(num3, num_sorted, tvstart, tvend, "insert");
  
    num4 = num;
    count = 0;
    gettimeofday( &tvstart, NULL);
    sort_quick(num4, 0, num4.size()-1);
    gettimeofday( &tvend, NULL);
    timespend = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
    check_sort_result(num4, num_sorted, tvstart, tvend, "quick");
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
