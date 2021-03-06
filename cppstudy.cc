

#if 1 // set/map operatons -- base in red-black tree
#include <iostream>
#include <string>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::multimap;
using std::set;

void printx(multimap<string, int> &iset)
{
    for (auto iter = iset.begin(); iter != iset.end(); iter++)
    {
        cout<<iter->first<<" "<<iter->second<<" ";
    }
    cout<<endl;
    return;
}

int main()
{
    multimap<string, int> imap = {{"waler",98}, {"carl", 99}, {"sam", 98}, {"boost",100}, {"waler",100}, {"carl", 66}};
   
    printx(imap);
    int count = imap.count("waler");
    auto iter5 = imap.find("waler");
    while (iter5!=imap.end())
    {
        cout<<"find "<<count<<" "<<iter5->first<<" "<<iter5->second;
        iter5++;
    }
    cout<<endl;
    
    for (auto beg = imap.lower_bound("waler"), end = imap.upper_bound("waler"); beg != end; ++beg)
    {
        cout<<"bound: "<<beg->first<<" "<<beg->second;
    }
    cout<<endl;

    set<int> iset = {1,4,0,99,8,2,3,10};
    iset.insert(999);
    auto iter1 = iset.lower_bound(8); // >=
    while (iter1 != iset.end())
    {
        cout<<*iter1++<<" ";
    }
    cout<<endl;

    auto iter2 = iset.upper_bound(8); // >
    while (iter2 != iset.end())
    {
        cout<<*iter2++<<" ";
    }
    cout<<endl;
    return 0;
}
#endif

#if 0
#include <iostream>
#include <memory>
#include <map>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::string;

void process(shared_ptr<int> p)
{
    *p = 3839;
    cout<<"func-in: "<<*p<<endl;
    return;
}

void printx(map<string, int> &m)
{
    cout<<"begin: "<<endl;
    for (auto iter = m.begin(); iter != m.end(); iter++)
    {
        cout<<iter->first<<" "<<iter->second<<endl;
    }
    return;
}

int main()
{
    int a = 0;
    int b(0);
    int c{0}; // only support in C++11
    int d = {0};

    shared_ptr<int> pi = make_shared<int>(1024);
    cout<<*pi<<endl;
    process(pi);
    cout<<*pi<<endl;
    
    shared_ptr<int> pi2(new int(3838));
    cout<<*pi2<<endl;

    int *x = new int(22);
    cout<<*x<<endl; 
    process(shared_ptr<int>(x));
    //memory of x has been freed after the invoke
    cout<<*x<<endl; // can't access pointer x

    map<string, int> imap;
    imap.insert({"hello", 11});
    printx(imap);
    imap.insert({"hellox", 2});
    printx(imap);
    imap.insert({"hello", 1});
    printx(imap);
    imap["hello"] = 1;
    printx(imap);

    auto iter = imap.find("hellox");
    cout<<"find result: "<<iter->first<<" "<<iter->second<<endl;
    return 0;
}
#endif


#if 0
#include <iostream>
#include <map>
#include <vector>
#include <string>
using std::map;
using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::vector;
using std::string;

int main()
{
    int array[] = {3,4,2};
    map<int, int> nums;
    vector<int> *pResult;
    string s1;
    int ret = 0;

    for (int i = 0; i < sizeof(array)/sizeof(int); i++)
    {
        auto iter = nums.find(6-array[i]);
        //if ((iter != nums.end()) && (i != iter->second))
        if (iter != nums.end())
        {
            pResult = new vector<int>{i, iter->second};
            cout<<i<<" "<<iter->second<<endl;
        }
        nums.insert({array[i], i});
    }
    s1 = "hello";
    // return value is unsigned int, so should not compare to 0
    if ((ret =s1.find("llx")) < 0)
        cout<<"don't contain ll "<<ret<<endl;
    if ((ret =s1.find("ll")) >= 0)
        cout<<"don't contain ll "<<ret<<endl;
    if (s1.find("ll") >= 0)
        cout<<"don't contain ll <="<<endl;
    if (s1.find("ll") >= 0)
        cout<<"don't contain ll >="<<endl;

    cout<<s1.find("ll")<<endl;
    cout<<s1.find("llx")<<endl;
    string s2 = s1.substr(1, s1.size()-1);
    cout<<s2<<endl;
    s2.clear();
    cout<<"after clear: "<<s2.size()<<endl;
    string s3("world");
    cout<<s1.size()<<" "<<s1<<endl;
    string s = s1+'a';
    cout<<s<<endl;

    string s4 = "hello";
    auto iter = s4.begin();
    s4.erase(++iter);
    cout<<s4<<endl;
    return 0;
}
#endif


#if 0 //friend 
#include <iostream>
using std::cout;
using std::endl;

void print();
class A {
    friend class B;
    friend int add(A &x);
    friend void print()
    {
        cout<<"this is a friend function"<<endl;
    }
private:
    int a;
    int b;
public:
    A():a(1), b(2) 
    { 
        print(); 
    }
    
    void show()
    {
        cout<<a<<" "<<b<<endl;
    }
};

class B {
private:
    int c;
    int d;
public:
    B():c(3), d(4) {}
    void clear(A &x)
    {
        x.a = 0;
        x.b = 0;
    }
};

int add(A &x)
{
    return x.a + x.b;
}

int main()
{
    A a;
    cout<<add(a)<<endl;
    B b;
    b.clear(a);
    a.show();
    return 0;
}
#endif

#if 0 // pure virtual function and abstract class.
#include <iostream>
using std::cout;
using std::endl;
class A 
{
    virtual void print() = 0;
};

class B: public A
{
    void print()
    {
        cout<<"B: print"<<endl;
    }

};

int main()
{
    //A oa;
    B ob;
    return 0;
}
#endif

#if 0 // conference and const member cann't be assigned.
#include <string>
using std::string;
template <typename T>
class NameObject {
    public:
        NameObject(string &name, const T &object): nameValue(name), objectValue(object) {}
    private:
        string &nameValue; //error 1
        const T objectValue;//error 2
};
int main()
{
   string newDog("walter"); 
   string oldDog("carl");
   NameObject<int> p(newDog, 22);
   NameObject<int> s(oldDog, 2);
   p = s;
   return 0;
}
#endif

#if 0 // copy constructor vs initalize list
#include <iostream>
using std::cout;
using std::endl;

class B
{
public:
    B(int &a)
    {
        a = a;
        cout<<"B construct a "<<a<<endl;
    }
    B(B &a)
    {
        a = a.a;
        cout<<"B construct a from copy "<<this->a<<endl;
    }
    B()
    {
        a = 0;
        cout<<"B default construct"<<endl;
    }
private:
    int a;
};

class A 
{
public:
    A(B &tmpb)  //before going to the function body, b has been initalized by invoked the default constructor.
    {
        b = tmpb; // this is assignment
    }
    A() = default;
    B b;    
};

class C 
{
public:
    C(B &tmpb): b(tmpb) //
    {
    }
    C() = default;
    B b;    
};

int main()
{
    B b;
    //A a(b);
    C c(b);
    return 0;
}


#endif
#if 0 // variable function parameters
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

template <typename T>
void printv(const T &arg1)
{
    cout<<" "<<arg1<<endl;
}

template <typename T, typename ... Args>
void printv(const T &arg1, const Args& ... rest)
{
    cout<<" "<<arg1<<" ";
    printv(rest...);
}

int main()
{
    printv("hello");
    printv("hello", 1);
    printv("hello", 1, 2.0);
    return 0;
}

#endif

#if 0 // new/delete
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
    auto pi = new int; // return a int* pointer.
    cout<<*pi<<endl;
    
    auto pi1 = new int();
    cout<<*pi1<<endl;
    
    auto p = new int[3]{0,2,3};
    cout<<*p<<*(p+1)<<*(p+2)<<endl;

    auto p1 = new string[3]{"helloxx", "world", "good"};
    cout<<*p1<<*(p1+1)<<*(p1+2)<<endl;

    string *p2 = p1;
    cout<<p2->size()<<endl;
    cout<<(p2+1)->size()<<endl;
    cout<<(p2+2)->size()<<endl;
   
    delete pi;
    delete pi1;
    delete [] p;
    delete [] p1;
    return 0;
}
#endif

#if 0 // IO API
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::string;
using std::endl;

int main()
{
   string s1,s2,s3;
   cin>>s1;
   cout<<s1<<endl;
   cin>>s2>>s3; //read first input to s2, second to s3; each input separates by whitespace
   cout<<s2<<s3<<endl;

   string word;
   while (cin>>word)
       cout<<word<<endl;
   return 0;
}
#endif

#if 0 //string operations
#include <iostream>
#include <string>
#include <string.h>

using std::cout;
using std::endl;
using std::string;

int main()
{
    string s1 = "hello wolrd"; //not including null
    cout<<s1<<endl;
    cout<<"len "<<s1.size()<<endl;

    //init
    string ss1 = s1;  //copy init
    string ss2(s1);   //direct init
    string ss3 = string(10, 'a'); //copy init
    cout<<ss3<<endl;
    string ss4(10, 'b'); //direct init
    cout<<ss4<<endl;

    //s(s1,pos,len)
    cout<<"s(s1, pos, len): "<<endl;
    string ss5(s1, 2);
    cout<<ss5<<endl;
    string ss6(s1, 2, 3);
    cout<<ss6<<endl;
   
    //operations +/>/==/
    cout<<"opetations for string:"<<endl;
    string so1("hello ");
    string so2("world ");
    string so3 = so1 + so2;
    cout<<so3<<endl;

    //substr(pos, len)
    cout<<"substr: "<<endl;
    string s2 = s1.substr(0, 2);
    cout<<s2<<endl;
    string s3 = s1.substr(2);
    cout<<s3<<endl;
    string s4 = s1.substr(3, 12);
    cout<<s4<<endl;
    try {
        string s5 = s1.substr(12);
    }
    catch(...) {
        cout << "Found throw." << endl; // Found throw.
    }

    char a[] = "fuck";
    cout<<sizeof(a)<<" "<<strlen(a)<<endl;
    string sa1(a); //not including the terminating '\0' character
    cout<<sa1.size()<<" "<<sa1<<endl;
    
    char b[] = {'b', 'l', 'u', 'e'};
    cout<<sizeof(b)<<" "<<strlen(b)<<endl;
    string sa4(b);
    cout<<sa4.size()<<" "<<sa4<<endl;
    string sa5 = b;
    cout<<sa5<<endl;
    return 0;
}
#endif

#if 0
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
class A
{
public:
    int val;
    float val2;
    char val3;
    long val4;
    static int static_val;
    string arr;
    //A *pnext;
};
A a;
int main()
{
    A b;
    cout<<"val "<<b.val<<endl;
    cout<<"val2 "<<b.val2<<endl;
    cout<<"val3 "<<b.val3<<endl;
    cout<<"val4 "<<b.val4<<endl;
    cout<<"arr "<<b.arr<<endl;
    cout<<"static val "<<b.static_val<<endl;
   
    cout<<"global val "<<a.val<<endl;
    cout<<"global val2 "<<a.val2<<endl;
    cout<<"global val3 "<<a.val3<<endl;
    cout<<"glocal val4 "<<a.val4<<endl;
    cout<<"global arr "<<a.arr<<endl;
    cout<<"global static "<<a.static_val<<endl;
    
    return 0;
}
#endif
#if 0 // string object
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main()
{
    string s0;
    cout<<s0<<" "<<s0.size()<<" "<<sizeof(s0)<<endl;
    if (s0.empty())
        cout<<"empty string"<<endl;

    string s1 = "hello";
    //string s1("hello");
    cout<<s1<<" "<<s1.size()<<" "<<sizeof(s1)<<endl;

    string s2(s1);
    cout<<s2<<endl;
    string s3(10, 's');
    cout<<s3<<endl;
    string s4 = string(10, 'w');
    cout<<s4<<endl;

    string s5(" world");
    string s6(s1+s5);
    cout<<s6<<endl;
    //os<<s6;
    return 0;
}
#endif

#if 0 //exception throw
#include <iostream>

using namespace std;

void Throw() { throw 1; }
void NoBlockThrow() { Throw(); }
void BlockThrow() noexcept { Throw(); }

int main() 
{
    try {
        Throw();
    }
    catch(...) {
        cout << "Found throw." << endl; // Found throw.
    }

    try {
        NoBlockThrow();
    }

    catch(...) {
        cout << "Throw is not blocked." << endl; // Throw is not blocked.
    }

    try {
        BlockThrow(); // terminate called after throwing an instance of 'int'
    }

    catch(...) {
        cout << "Found throw 1." << endl;
    }
}

#endif
#if 0
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using std::cout;
using std::endl;
using std::vector;
using std::string;

int main()
{
    vector<int> vec = {1,2,3,4,5,6,7,8,10};
    vector<int> vec2 = {1,2,3,4,5,6,7,8,10,11,12};
    vector<string> vecStr = {"hello", "world", "fuck", "you", "then"};
    int val = 10;
    auto result = find(vec.begin(), vec.end(), val);
    cout<<"value "<<val<<(result == vec.end() ? " is not present" : " is present")<<endl;
   
    //find in the subrange; the range is [ )
    val = 7;
    auto iter = vec.begin();
    result = find(iter+3, iter+6, val);
    cout<<"value "<<val<<(result == iter+6 ? " is not present" : " is present")<<endl;
    cout<<*result<<endl;

    int sum = accumulate(vec.begin(), vec.end(), 0);
    cout<<"sum "<<sum<<endl;

    string sumStr = accumulate(vecStr.begin(), vecStr.end(), string(""));
    cout<<"sum string "<<sumStr<<endl;

    auto ret = equal(vec.begin(), vec.end(), vec2.begin());
    cout<<"equal "<<ret<<endl;
    return 0;
}
#endif
#if 0
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using std::cout;
using std::endl;
using std::string;
using std::vector;

class MetaData
{
public:
    //MetaData() = default; 
    MetaData (int size, const std::string& name): _name( name ), _size( size ){}

    // copy constructor
    MetaData (const MetaData& other): _name( other._name ), _size( other._size ){ cout<<"copy"<<endl; }

    // move constructor2
    //MetaData (MetaData&& other): _name( std::move(other._name) ), _size( other._size ){ cout<<"move"<<endl; }
    MetaData (MetaData&& other): _name( other._name ), _size( other._size ){ cout<<"move"<<endl; }

    std::string getName () const { return _name; }
    int getSize () const { return _size; }

private:
    std::string _name;
    int _size;
};

#if 1
class ArrayWrapper
{
public:
    // default constructor produces a moderately sized array
    ArrayWrapper (): _p_vals( new int[ 64 ] ), _metadata( 64, "ArrayWrapper" )
    {
        cout<<"default constructor"<<endl;
    }

    ArrayWrapper (int n): _p_vals( new int[ n ] ), _metadata( n, "ArrayWrapper" )
    {
        cout<<"n constructor"<<endl;
    }

    // move constructor
    ArrayWrapper (ArrayWrapper&& other): _p_vals( other._p_vals  ), _metadata( std::move(other._metadata) )
    //ArrayWrapper (ArrayWrapper&& other): _p_vals( other._p_vals  ), _metadata( other._metadata )
    {
        cout<<"move constructor"<<endl;
        other._p_vals = NULL;
    }

    // copy constructor
    ArrayWrapper (const ArrayWrapper& other): 
                 _p_vals( new int[ other._metadata.getSize() ] ), 
                 _metadata( other._metadata )
    {
        cout<<"copy constructor"<<endl;
        for ( int i = 0; i < _metadata.getSize(); ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
    }
    
    ~ArrayWrapper ()
    {
        cout<<"destructor"<<endl;
        delete [] _p_vals;
    }
private:
        int *_p_vals;
        MetaData _metadata;
};
#endif

#if 0
class ArrayWrapper
{
public:
    // default constructor produces a moderately sized array
    ArrayWrapper (): _p_vals( new int[ 64 ] ), _size( 64 ){ cout<<"default"<<endl; }
    ArrayWrapper (int n): _p_vals( new int[ n ] ), _size( n ){ cout<<"n constructor"<<endl; }

    // move constructor
    ArrayWrapper (ArrayWrapper&& other): _p_vals( other._p_vals  ), _size( other._size )
    {
        cout<<"move"<<endl;
        other._p_vals = NULL;
        other._size = 0;
    }

    // copy constructor
    ArrayWrapper (ArrayWrapper& other): _p_vals( new int[ other._size  ] ), _size( other._size )
    {
        cout<<"copy"<<endl;
        for ( int i = 0; i < _size; ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
    }
    
    ~ArrayWrapper ()
    {
        cout<<"destructor"<<endl;
        //if (_p_vals)
            delete [] _p_vals;
    }

private:
    int *_p_vals;
    int _size;
};
#endif

ArrayWrapper getArray(ArrayWrapper &a)
{
    return a;
}

int main()
{
    ArrayWrapper st;
    //const ArrayWrapper &lva = getArray(st); 
    //ArrayWrapper &&rva = getArray(st); 
    //ArrayWrapper st2(std::move(st));
    ArrayWrapper st2(getArray(st));
    //ArrayWrapper st2(std::move(getArray(st)));

    //vector<ArrayWrapper> ArrayVec;
    //ArrayVec.push_back(ArrayWrapper(2));
    return 0;
}
#endif

#if 0 // rvalue ref and move constructor
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

string printstr(void)
{
    return "fuck you then";
}

void printRef(const string &s)
{
    cout<<"lvalule ref version: "<<s<<endl;
}

void printRef(const string &&s)
{
    cout<<"rvalule ref version: "<<s<<endl;
}

int main()
{
    string a("hello world");
    printRef(a); 
    printRef(printstr()); //only temp object calls the move-function

    int i = 0;
    int &lref = i;
    int &&rref = i+1;
    //int &&rref2 = rref; // rref is a lvaule itself
    return 0;
}
#endif

#if 0 // container adaptor: stack, queue, priority queue
#include <iostream>
#include <stack>
#include <queue>
using std::cout;
using std::endl;
using std::stack;
using std::queue;

int main(void)
{
    // stack
    stack<int> iStack; // what relationship between stack and sequential container
                       // why use deque to construct the stack, why not use vector?

    for (int idx = 0; idx < 10; idx++)
    {
        iStack.push(idx);
    }
    
    while (!iStack.empty())
    {
        cout<<iStack.top()<<" "<<&(iStack.top())<<endl; // why does stack grow from low to high?
                                                        // maybe it's just a app layer statck, not stack in memory layer
        iStack.pop();
    }

    // queue and priority queue
    queue<int> iQueue;
    for (int idx = 0; idx < 10; idx++)
    {
        iQueue.push(idx);
    }
    while (!iQueue.empty())
    {
        cout<<iQueue.front()<<" "<<&(iQueue.front())<<endl; 
        iQueue.pop();
    }

    return 0;
}
#endif

#if 0
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::allocator;

class StrVec {
public:
   StrVec(): elements(nullptr), first_free(nullptr), cap(nullptr) {}
   StrVec(const StrVec &) {}
   StrVec& operator=(const StrVec &) {}
   StrVec* operator++(const StrVec *src) { return src + src->size(); }
   ~StrVec() {}

   void push_back(const string &);
   size_t size() { return first_free - elements; }
   size_t capacity() { return cap - elements; }
   string* begin() { return elements; }
   string* end() { return first_free; }

private:
    allocator<string> alloc;
    void chk_n_alloc() {}
    //pair<string*, string*> alloc_n_copy() {}
    void free() {}
    void reallocate() {}

    string *elements;
    string *first_free;
    string *cap;
};

#define MAX_BUFF 128
void StrVec::push_back(const string &s)
{
    cout<<first_free<<endl;
    if (elements == cap) //null vec
    {
        elements = alloc.allocate(MAX_BUFF);
        alloc.construct(elements, s);
        first_free = elements + s.size();
    }
    else
    {
        alloc.construct(first_free, s);
        first_free = first_free + s.size();
    }
}

int main()
{
    StrVec vec;
    vec.push_back("hello world");
    vec.push_back("fuck you then");
    for (auto iter = vec.begin(); iter != vec.end(); iter++)
    {
        cout<<*iter<<endl;
    }
    return 0;
}
#endif

#if 0 //copy initialize for initialized_list
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class X {
public:
    X() { cout <<"X()"<<endl; }
    X(const X&) { cout<<"X(const X&)"<<endl; }
    X& operator = (const  X&) 
    { 
        cout<<"X& operator = (const X&)"<<endl; 
        return *this; 
    }
    ~X() { cout<< "~X()"<<endl; }
};

int main()
{
    X x = X();
    //vector<X> vec_x{ x };
    //vector<X> vec_x = {x};
}

#endif


#if 0 //copy control  class has valuelike and pointerlike members.

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

#if 1 //this is pointerlike
class HasPtr {
public:
    HasPtr(const string &s = string()): ps(new string(s)), i(0), use(new int(1)) {} // 1, Is this default constructor? 
    HasPtr(const HasPtr &p): ps(p.ps), i(p.i), use(*(p.use)){ (*use)++};
    HasPtr& operator=(const HasPtr &);
    ~HasPtr();
private:
    string *ps;
    int i;
    int *use;
};

HasPtr & HasPtr::operator=(const HasPtr &rhs)
{
    (++*(rhs.use));
    if (--(*use))
    {
        delete ps;
        delete use;
    }
    ps = rhs.ps;
    i = rhs.i;
    use = rhs.use;
    return *this;
}

HasPtr::~HasPtr()
{
    (*use)--;
    if (*use == 0)
    {
        delete ps;
        delete use;
    }
}
#endif
#if 0//this if valuelike
class HasPtr {
public:
    HasPtr(const string &s = string()): ps(new string(s)), i(0) {} // 1, Is this default constructor? 
                                                               //2, why use new to define a null string, why not use nullptr to assign the ps pointer
    //HasPtr(const HasPtr &p): ps(new string(*p.ps)), i(p.i){}
    HasPtr(const HasPtr &p);
    HasPtr & operator=(const HasPtr &);
    ~HasPtr(){delete ps;}
private:
    string *ps;
    int i; 
};

HasPtr & HasPtr::operator=(const HasPtr &rhs)
{
    auto newp = new string(*rhs.ps);
    delete ps;
    ps = newp;
    i = rhs.i;
    return *this;
}

HasPtr::HasPtr(const HasPtr &p)
{
    ps = new string(*p.ps);
    i = p.i;
}
#endif

int main()
{
    HasPtr temp;
    return 0;
}

#endif

#if 0 //move constructor
#include <iostream>

using std::cout;
using std::endl;

class A {
public:
    A(): a(1), b(2), p(new int[10]) {} // how to initalize the new int[] when define
    A(A &&s) noexcept : a(s.a), b(s.b), p(s.p) 
    {
        s.p = nullptr;
        cout<<"move"<<endl; 
    }
    A(const A &s): a(s.a), b(s.b), p(new int[10]) { cout<<"copy"<<endl; }
    void show() 
    { 
        cout<<a<<" "<<&a<<" "<<b<<" "<<&b<<" "<<p<<endl; 
    }
    ~A() 
    {
        if (p)
            delete p;
        cout<<"destructor"<<endl; 
    }
//private:
    int a;
    int b;
    int *p;
};

int main()
{
    A a;
    a.show();
    A b(a);
    b.show();
    A c(std::move(a));
    c.show();
    a.a = 10000; // a has been moved to c, why a and c have the different memory
    cout<<a.a<<" "<<c.a<<endl;
    return 0;
}
#endif

#if 0 //query word program
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::multimap;

#if 0 //c_style code
int main()
{
    vector<string> line;
    string word;
    string temp;
    multimap<string, int> imap;
    ifstream input("test3");
    istringstream str;

    while (getline(input, word))
    {
        line.push_back(word);
    }
    auto iter = line.begin();
    while (iter != line.end())
    {
        cout<<*iter<<endl;
        iter++;
    }

    //create the map map<string, line> from the vector
    for (int idex = 0; idex != line.size(); idex++)
    {
        istringstream str(line[idex]);
        while (str >> temp)
        {
            imap.insert(make_pair(temp, idex));
        }
    }
    auto iter2 = imap.begin();
    while (iter2 != imap.end())
    {
        cout<<iter2->first<<" "<<iter2->second<<endl;
        iter2++;
    }

    string query;
    cout<<"please input the word you want to find: "<<endl;
    cin>>query;

    auto num = imap.count(query);
    auto iter3 = imap.find(query);
    for (auto i = 0; i < num; i++)
    {
        cout<<"line:"<<iter3->second<<" "<<line[iter3->second]<<endl;
        iter3++;
    }
    return 0;    
}
#endif

#if 0 //c++_style code
class TextQuery 
{
    vector<string> line;
    multimap<string, int> imap;
public:
    TextQuery(string &);
    void Query(string &);
    void Show();
};

TextQuery::TextQuery(string &file_name)
{
    string word;
    string temp;
    ifstream input(file_name);
    while (getline(input, word))
    {
        line.push_back(word);
    }
    
    for (int idex = 0; idex != line.size(); idex++)
    {
        istringstream str(line[idex]);
        while (str >> temp)
        {
            imap.insert(make_pair(temp, idex));
        }
    }
}

void TextQuery::Show()
{
    auto iter = line.begin();
    while (iter != line.end())
    {
        cout<<*iter<<endl;
        iter++;
    }
}

void TextQuery::Query(string &query)
{
    auto num = imap.count(query);
    auto iter3 = imap.find(query);
    for (auto i = 0; i < num; i++)
    {
        cout<<"line:"<<iter3->second<<" "<<line[iter3->second]<<endl;
        iter3++;
    }
}

int main()
{
    string name;
    string word;
    cout<<"please input the file name: "<<endl;
    cin>>name;
    TextQuery a(name);
    a.Show();

    cout<<"please input the word your want to find: "<<endl;
    cin>>word;
    a.Query(word);

    int i = 3;
    int &refi = i;
    //int &refi2 = i * 2;
    const int &refi3 = i * 2;

    return 0;
}
#endif
#endif

#if 0 //allocator => memory pool
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::allocator;
using std::uninitialized_fill_n;

int main()
{
    allocator<int> al;
    auto p1 = al.allocate(2); //only alloc memory, not construct
    auto p2 = p1; // save the pointer to the first element
    al.construct(p2++, 1234);
    cout<<p2<<" "<<*p2<<endl;
    al.construct(p2++, 5678);
    cout<<p2<<" "<<*p2<<endl;

    while (p2 != p1)
    {
        cout<<"destory p3 "<<p3<<endl;
        al.destroy(p3++);
    }
    al.deallocate(p1, 2); //first para must be the return of allocate()

    return 0;
}
#endif

#if 0 //type conversion
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    double dval = 3.14;
    const int &ri = dval;
    //int &ri = dval; //error
    cout<<ri<<endl;
    return 0;
}
#endif

#if 0 // smart pointer 2
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Controller 
{
    public:
        int Num;
        wstring Status;
        vector<weak_ptr<Controller>> others;
        explicit Controller(int i) : Num(i) , Status(L"On")
        {
            wcout << L"Creating Controller" << Num << endl;
        }

        ~Controller()
        {
            wcout << L"Destroying Controller" << Num << endl;
        }


        void CheckStatuses() const
        {
            for_each(others.begin(), others.end(), [] (weak_ptr<Controller> wp)
            {
                try
                {
                    auto p = wp.lock();
                    wcout << L"Status of " << p->Num << " = " << p->Status << endl;
                }

                catch (bad_weak_ptr b)
                {
                    wcout << L"Null object" << endl;
                }                
            });
        }
};

void RunTest()
{
    vector<shared_ptr<Controller>> v;

    v.push_back(shared_ptr<Controller>(new Controller(0)));
    v.push_back(shared_ptr<Controller>(new Controller(1)));
    v.push_back(shared_ptr<Controller>(new Controller(2)));
    v.push_back(shared_ptr<Controller>(new Controller(3)));
    v.push_back(shared_ptr<Controller>(new Controller(4)));


    for (int i = 0 ; i < v.size(); ++i)
    {
        for_each(v.begin(), v.end(), [v,i] (shared_ptr<Controller> p)
        {
            if(p->Num != i)
            {
                v[i]->others.push_back(weak_ptr<Controller>(p));
                wcout << L"push_back to v[" << i << "]: " << p->Num << endl;
            }
        });        
    }

    for_each(v.begin(), v.end(), [](shared_ptr<Controller>& p)
    {
        wcout << L"use_count = " << p.use_count() << endl;
        p->CheckStatuses();
    });
}

int main()
{    
    RunTest(); 
    wcout << L"Press any key" << endl;
    char ch;
    cin.getline(&ch, 1);
}

#endif

#if 0 //smart pointer 2
#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;

int main()
{
    shared_ptr<int> spi(new int(1));

    shared_ptr<int> spi2 = make_shared<int>(222);
    cout<<*spi2<<endl;
    return 0;
}
#endif

#if 0 //smart pointer
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;

shared_ptr<int> factory(int a)
{
    return make_shared<int>(a);
}

void print(shared_ptr<int> p)
{
    cout<<"in print func "<<p.use_count()<<endl;
    cout<<"p "<<p<<" p value "<<*p<<endl;
}

void print_ref(shared_ptr<int> &p)
{
    cout<<"in print_ref func "<<p.use_count()<<endl;
    cout<<"p "<<p<<" p value "<<*p<<endl;
}

int main()
{
    shared_ptr<string> p1 = make_shared<string>(10, '9');
    unique_ptr<string> p11(new string("hello world")); //p2 is 0 if it has not pointer to a object
   
    // size of the smart pointer
    cout<<"size of smart pointer(shared_ptr): "<<sizeof(p1)<<" => raw int * size: "<<sizeof(int *)<<endl;//why 16 bytes
    cout<<"size of smart pointer(unique_ptr): "<<sizeof(p11)<<" => raw int * size: "<<sizeof(int *)<<endl;//why 16 bytes

    // get()
    cout<<p1<<endl;
    cout<<p1.get()<<endl;
    cout<<sizeof(p1.get())<<endl;

    string *p12 = new string("fuck");
    shared_ptr<string> p13(p12);
    cout<<p12<<endl;
    cout<<p13<<endl;
    cout<<p13.get()<<endl;
   
    // reset()
    p11.reset(); //free the memory just now.
    //cout<<*p11<<endl; //this is error, because the memory p11 pointed has been already freed.

    auto q(p1);
    cout<<*q<<endl;
    cout<<p1.use_count()<<endl;
    cout<<q.use_count()<<endl;
   
    auto r = make_shared<string>();
    q = r;
    cout<<p1.use_count()<<endl;
    cout<<r.use_count()<<endl;
    cout<<sizeof(p1)<<endl;//why 16 bytes

    auto p3 = factory(1);
    cout<<p3.use_count()<<endl;

    shared_ptr<int> i(new int(4));
    cout<<"i "<<i<<" count "<<i.use_count()<<endl;
    print(i);
    cout<<i.use_count()<<endl;
    print_ref(i);
    cout<<i.use_count()<<endl;

    int *p(new int(4));
    print(shared_ptr<int>(p));
    int j = *p; //this is dangerous
    //free(p); //this will cause a run error: p has been freed twice.

    shared_ptr<int> p4(new int(4)); //what difference between new and make_shared to assign to shared_ptr
    //shared_ptr<int> p4 = make_shared<int>(4);
    int *q4 = p4.get();
    {
        shared_ptr<int>(q4);
    }
    auto a = *p4; //this is dangerous! memory has been deleted already.

    shared_ptr<int> p5(new int(38));
    if (p5.unique())
        p5.reset(new int(1));//does reset decreases the reference of the old pointed memory for p5;

    cout<<*p5<<endl;

    //unique_ptr
    //vector<unique_ptr<string>> pvec;
    //unique_ptr<string> pvec1(new string("hello"));
    vector<shared_ptr<string>> pvec;
    pvec.push_back(make_shared<string>("hello"));
    pvec.push_back(make_shared<string>("world"));
    for (auto vec : pvec)
        cout<<*vec<<endl;

    vector<unique_ptr<string>> pvec2;
    unique_ptr<string> pvec3(new string("hello"));
    pvec2.push_back(std::move(pvec3));
    for (const auto& vec2 : pvec2) //why use reference
        cout<<*vec2<<endl;
    for (auto iter = pvec2.begin(); iter != pvec2.end(); iter++)
        cout<<*(iter->get())<<endl;

    //weak_ptr
    shared_ptr<string> shP = make_shared<string>("hello world");
    shared_ptr<string> shP2 = shP;
    shared_ptr<string> shP3(shP2);
    weak_ptr<string> weP(shP);
    cout<<"shared_ptr count: "<<shP.use_count()<<" weak_ptr count: "<<weP.use_count()<<endl;

    if (!weP.expired())
    {
         auto shP3 = weP.lock();
    }
    return 0;
}
#endif

#if 0 //word transport program
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::map;

// get one line, use getline()
// get one word use >> and istringstream

int main(int argc, char **argv)
{
    ifstream map_file("test1");
    ifstream input_file("test2");
    string key, value, text, word;
    map<string, string> trans_map;

    /* create the transport map */
    while (map_file>>key && getline(map_file, value))
    {
        if (value.size() > 1)
        {
            trans_map[key] = value.substr(1);
        }
        else
        {
            cout<<"wrong format of trans file"<<endl;
        }
    }

    for (auto iter = trans_map.begin(); iter != trans_map.end(); iter++)
    {
        cout<<iter->first<<"=>"<<iter->second<<endl;
    }

    while (getline(input_file, text))
    {
        cout<<"original text: "<<text<<endl;
        istringstream streamtmp(text);
        cout<<"after convert: ";
        while (streamtmp>>word)
        {
            auto iter1 = trans_map.find(word);
            if (iter1 != trans_map.end())
            {
                cout<<iter1->second;
            }
            else
            {
                cout<<word<<" ";
            }
        }
        cout<<endl;
    }
    
    return 0;
}
#endif

#if 0 //unordered_map: a hash struct ?
#include <iostream>
#include <string>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;

int main()
{
    unordered_map<string, int> imap;
    string word;

    while (cin>>word)
    {
        //imap.insert(make_pair(word, 1));
        ++imap[word]; // if the element doesn't exist, insert it.
    }
    
    for (auto beg = imap.begin(); beg != imap.end(); beg++)
    {
        cout<<beg->first<<" "<<beg->second<<endl; //it seems that the order is inverted for the input.
    }
   
    cout<<"map size: "<<imap.size()<<endl;
    cout<<"bucket number: "<<imap.bucket_count()<<endl;
    cout<<"max bucket number supported: "<<imap.max_bucket_count()<<endl;
    cout<<"average number of elements per bucket: "<<imap.load_factor()<<endl;
    cout<<"max average number of elements per bucket: "<<imap.max_load_factor()<<endl;
    for (auto i = 0; i < imap.bucket_count(); i++)
    {
        cout<<"bucket "<<i<<" size: "<<imap.bucket_size(i)<<endl;
        for (auto j = imap.begin(i); j != imap.end(i); j++)
        {
            cout<<"bucket "<<i<<" element "<<j->first<<" "<<j->second<<endl;        
        }
    }
    return 0;
}
#endif

#if 0 //file input and output //ifstream ofstream fstream
      // istream ifstream istringstream
      // ostream ofstream ostringstream
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::istream;
using std::ofstream;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::flush;
using std::ends;

struct PeopleInfo 
{
    string name;
    vector<string> phones;
};

void input(istream &i) // must be reference
{
    int a;
    while(i>>a)
        cout<<"input is "<<a<<endl;
}

void printv(const vector<PeopleInfo> &t)
{
    for (auto i = t.begin(); i != t.end(); i++)
    {
        cout<<i->name<<endl;
        for (auto j = i->phones.begin(); j != i->phones.end(); j++)
        {
            cout<<*j<<" ";
        }
        cout<<endl;
    }

    for (auto &entry: t)
    {
        ostringstream info;
        info<<entry.name<<" ";
        for (auto &p: entry.phones)
        {
            info<<p<<" ";
        }
        cout<<info.str(); // don't display right now until meeting the buff flush 
        sleep(3);
        //cout<<endl; // invoke endl to flush the output buff
        //cout<<flush;
        cout<<ends;
    }
}

int main()
{
    vector<PeopleInfo> people;
    ifstream in("test");
    //ofstream out("output");
    string word, line;
    
    while (getline(in, line))
    {
        PeopleInfo peopleinfo;
        istringstream istr(line);
        istr>>peopleinfo.name;
        while (istr>>word)
        {
            peopleinfo.phones.push_back(word);
        }
        people.push_back(peopleinfo);
    }
    printv(people);

    input(cin);
    return 0;
}
#endif

#if 0 
#include <iostream>
#include <string>
#include <iterator>
#include <unistd.h>
#include <vector>
#include <initializer_list>
//using namespace std; // don't use this style code, that may cause name conflict
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
//using std::initializer_list;
#endif

#if 0      // map / set
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::set;
using std::multiset;
using std::map;
using std::multimap;
using std::pair;

int main()
{
   map<string, int> count;
   set<string> exclude = {"hello"};
   string word;

   exclude.insert("fuck");
   while (cin>>word)
   {
       if (exclude.find(word) == exclude.end())
           ++count[word];
   }
   
   for (const auto &w : count)
   {
       cout<<w.first<<" occurs "<<w.second<<((w.second > 1) ? "times" : "time")<<endl;
   }

   //iterator of map is a pair type
   auto map_it = count.begin();
   while (map_it != count.end())
   {
       cout<<map_it->first<<"  "<<map_it->second<<endl;
       map_it++;
   }

   vector<int> ivec;
   for (vector<int>::size_type i = 0; i < 10; i++)
   {
       ivec.push_back(i);    
       ivec.push_back(i);    
   }

   set<int> iset(ivec.cbegin(), ivec.cend());
   multiset<int> miset(ivec.cbegin(), ivec.cend()); // cbegin() return const_iterator

   cout<<ivec.size()<<endl;
   cout<<iset.size()<<endl;
   cout<<miset.size()<<endl;
 
   multimap<string, int> map2 = {{"hello", 1}, {"world", 1}, {"fuck", 1}, {"you", 1}, {"then", 1}};
   auto iter2 = map2.insert(std::make_pair("hello", 0));
   cout<<"map2: "<<iter2->first<<" "<<iter2->second<<endl;
  
   //find and count
   auto cou = map2.count("hello");
   cout<<"hello number: "<<cou<<endl;
   auto iter3 = map2.find("hello");
   while (cou)
   {
       cout<<"map3: "<<iter3->first<<" "<<iter3->second<<endl;
       iter3++;
       cou--;
   }

   //lower_bound and upper_bound
   for (auto beg = map2.lower_bound("hello"), end = map2.upper_bound("hello"); beg != end; beg++)
   {
       cout<<"map3 2: "<<beg->first<<" "<<beg->second<<endl;
   }

   auto iter4 = map2.lower_bound("halat");
   cout<<"iter4: "<<iter4->first<<" "<<iter4->second<<endl;
   auto iter5 = map2.upper_bound("halat");
   cout<<"iter5: "<<iter5->first<<" "<<iter5->second<<endl;

   //equal_range  reutrn a pair 
   for (auto pos = map2.equal_range("hello"); pos.first != pos.second; ++(pos.first))
   //for (pair<multimap<string, int>::iterator, multimap<string, int>::iterator> pos = map2.equal_range("hello"); 
   //     pos.first != pos.second; ++(pos.first))
   {
       cout<<"iter6: "<<pos.first->first<<" "<<pos.first->second<<endl;
       cout<<"iter6: "<<pos.second->first<<" "<<pos.second->second<<endl;
   }

   //delete elements with the specified key.
   auto num = map2.count("hello");
   if (num > 0)
   {
       cout<<"erase element "<<map2.erase("hello")<<endl;
   }
   cout<<"after erase hello: "<<endl;
   for (auto iter9 = map2.begin(); iter9 != map2.end(); iter9++)
   {
       cout<<"iter9 "<<iter9->first<<" "<<iter9->second<<endl;
   }

   auto iter7 = map2.find("world");
   if (iter7 != map2.end())
   {
       auto iter8 = map2.erase(iter7);
       cout<<"after erase, iter denote "<<iter8->first<<" "<<iter8->second<<endl;
   }
   cout<<"after erase world: "<<endl;
   for (auto iter10 = map2.begin(); iter10 != map2.end(); iter10++)
   {
       cout<<"iter10 "<<iter10->first<<" "<<iter10->second<<endl;
   }
   return 0;
}
#endif

#if 0 //pair
#include <iostream>
#include <vector>
#include <utility>

using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
int main()
{
   pair<int, int> a(1, 2);
   pair<string, string> is("hello", "world");
   cout<<a.first<<endl;
   cout<<a.second<<endl;
   cout<<is.first<<endl;
   cout<<is.second<<endl;
   //vector<string> ss = {"hello", "world"};

   auto b = make_pair(1, 1);
   cout<<b.first<<endl;
   cout<<b.second<<endl;

   if (a < b)
       cout<<"a < b"<<endl;
   else
       cout<<"a >= b"<<endl;

   pair<int, string> c(1, "hello");
   cout<<c.first<<endl;
   cout<<c.second<<endl;
   //c.first = "world"; //error: c.first is a const type
   return 0;
}
#endif

#if 0 // thread-safe class
class Counter
{
public:
    Counter(): value_(0){};
    Counter(int a): value_(a){};
    int value() const;
    int getAndIncrease();
private:
    int value_;
};

int Counter::value() const
{
    return value_;
}

int Counter::getAndIncrease()
{
    int ret = value_++;
    return ret;
}

int main()
{
    Counter ci(2);
    cout<<ci.value()<<endl;
    return 0;
}

#endif

#if 0 // smart pointer
int func(initializer_list<string> li)
{
    for (auto err = li.begin(); err != li.end(); err++)
    {
        cout<<*err<<endl;
    }
    return 0;
}

int main(int argc, char **argv)
{
    //func({"hello", "world"});
    return 0;
}
#endif

#if 0 // operator
class person{
    private:
        int age;
    public:
        person(int a){
            this->age=a;
        }
        inline bool operator==(const person &ps) const;
};

inline bool person::operator==(const person &ps) const
{
    if (this->age == ps.age)
    {
        cout<<"here"<<endl;
        return true;
    }
    return false;
}

int main()
{
    person p1(10);
    person p2(10);
    
    if (p1 == p2) 
        cout<<"the age is equal 0"<<endl;

    return 0;
}
#endif

#if 0 // template class for a link and static member in template class
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <class Type> class Queue; 
template <class Type> class QueueItem { 
    friend class Queue<Type>;
    QueueItem(const Type &t): item(t), next(0) { }  
    Type item;      
    QueueItem *next;      
};

template <class Type> class Queue {  
    public:  
        //Queue(): head(0), tail(0), size(0) { } 
        Queue(): head(0), tail(0) { } 
        void push(const Type &);        
        void pop();                      
        bool empty() const 
        {            
            return (head == 0);  
        }
        void print()
        {
            QueueItem<Type> *pItem = head;
            cout<<"link item number: "<<size<<endl;
            if (!empty())
            {
                while (pItem)
                {
                    cout<<pItem->item<<endl;
                    pItem = pItem->next;
                }
            }
        }
        //void init_size(unsigned val){size = val;}

    private:  
        QueueItem<Type> *head;           
        QueueItem<Type> *tail;    
        static unsigned size;
        //unsigned size;
};

template <class Type> unsigned Queue<Type>::size = 0;

template <class Type> void Queue<Type>::pop()
{
    QueueItem<Type> *p = head;
    delete(p);
    head = head->next;
    size--;
}

template <class Type> void Queue<Type>::push(const Type &val)
{
    QueueItem<Type> *p = new QueueItem<Type>(val);
    if (empty())
    {
        head = tail = p;
    }
    else
    {
        tail->next = p;
        tail = p;
    }
    size++;
}

int main(void)
{
    Queue<int> oa;
    oa.push(2);
    oa.push(3);
    oa.print();
    oa.pop();
    oa.print();
    Queue<int> oa1;
    oa1.push(38);
    oa1.print();
    
    Queue<string> ob;
    string s("hello");
    ob.push(s);
    ob.print();

    return 0;
}
#endif

#if 0 // implicit invert
int main(void)
{
    int i = 0;
    const int j = 1;
    const int &ci = i; 
    const int &cj = j;
    const int *pi = &i; 
    const int *pj = &j;
    //int &ck = j;  //error 
    //int *pk = &j;  //error
    //int array[i] = {0};
    unsigned cnt = 2;
    int array[cnt];
    string arraystr[cnt];
    array[0] = 1;

    cout<<array[i]<<endl;
}
#endif

#if 0 // template function
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <typename T>
int compare(T a, T b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

//template <typename T>
//int compare_ref(T &a, T &b)
int compare_ref(char * &a, char * &b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

template <unsigned N, unsigned M>   // nontype template parameter, represent a value rather than a type
int comp(const char (&p)[N], const char (&q)[M])
{
    cout<<"N: "<<N<<"M: "<<M<<endl;
    return strcmp(p, q);
}

int main()
{
    int a = 1, b = 2;
    string s1("hello world."), s2("fuck you then");
    char s3[] = "hello world";
    char s4[] = "fuck you then";
    cout<<compare(s1, s2)<<endl;
#if 0
    cout<<compare(s3, s4)<<endl;
    cout<<compare(a, b)<<endl;
    cout<<comp("hello", "world")<<endl;
    cout<<comp(s4, s3)<<endl;
    //cout<<comp(s1, s2)<<endl;   // error, string isn't equal to char a[]
    //cout<<compare_ref(s1, s2)<<endl;
    cout<<compare_ref(s3, s4)<<endl;  //error, char [12] not equal to char [14]
#endif
    return 0;
}
#endif

#if 0 // memory layout for the base and derived class
class base
{
public:
    int a;
    int b;
    virtual void print()
    {
        cout<<"base"<<endl;
    }
};

class derived: public base
//class derived: protected base
{
public:
    int c;
};

int main()
{
    base a;
    derived b;

    cout<<sizeof(a)<<endl;
    cout<<sizeof(b)<<endl;
    cout<<&b<<endl;
    cout<<&(b.a)<<endl;
    
    return 0;
}
#endif

#if 0
/* function override and overload 
 * */
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Base 
{
    public:
        virtual void print() {
            cout << "print() in Base." << endl;
        }
        void print(int a) {
            cout << "print(int a) in Base." << endl;
        }
        void print(string s) {
            cout << "print(string s) in Base." << endl;
        }
};

//class Derived : public Base { };
class Derived : public Base 
{
    public:
        using Base::print;
        void print() {
            cout << "print() in Derived." << endl;
        }
};

class DerivedSon : public Derived 
{
    public:
        //void print(){
        void print() const {   // const overload
            cout << "print() in DerivedSon." << endl;
        }
};

int main() {
    Derived d;
    d.print();
    d.print(10);
    d.print("");
    //d.Base::print(10);
    //d.Base::print("");
    d.Base::print();

    Base &pBase = d;
    pBase.print();
    pBase.Base::print();
    //pBase.Derived::print();

    DerivedSon s;
    s.print();
    Derived &pDerived = s;
    pDerived.print();
    return 0;
}
#endif

#if 0 //function overload 2
#include <iostream>
using std::cout;
using std::endl;

class Base{
    public:
        int display(int n_count){
            cout <<"display function defined in the Class Base"<<endl;
            return 0;
        }
};

class Derived: public Base{
    public:
        //using Base::display;
        int display(int n_count,int n_width){
            cout << "display function defined in the Class Derived"<<endl;
            return 0;
        }
};

int main(int argc,char** argv)
{
    Base *p_base;
    Derived *p_derived = new Derived();
    p_base = p_derived;
    //p_derived->display(8);
    p_derived->display(10,20);
    p_base->display(8);
    return 0;
}
#endif

#if 0 //function overload
#include <iostream>
using std::cout;
using std::endl;

#if 0
void print(char i)
{
    cout<<"int "<<i<<endl;
    return;
}

int print(int i)
{
    cout<<"int "<<i<<endl;
    return 0;
}
#endif

long print(long i)
{
    cout<<"long "<<i<<endl;
    return 0;
}

int main()
{
    print(static_cast<long>(38));
    print('a'); //error
    return 0;
}
#endif

#if 0  // destructor should be virtual
#include <iostream>
using std::cout;
using std::endl;

class A {
public:
        A();
        virtual ~A();
        //~A();
};

A::A()
{
    cout<<"A constructor"<<endl;
}

A::~A()
{
    cout<<"A destructor"<<endl;
}

class B : public A {
public:
        B();
        ~B();
};

B::B()
{ 
    cout<<"B constructor"<<endl;
}

B::~B()
{
    cout<<"B destructor"<<endl;
}

int main(int argc, char* argv[])
{
    A *b=new B;
    delete b;
    return 0;
}
#endif

#if 0 //defalut constrctor and public/protected inherit
#include <iostream>
using std::cout;
using std::endl;

class A{
public:
    int a;
    A() = default;
    A(int c, int d): a(c),b(d){};
    void printx()
    {
        cout<<"class a "<<b<<endl; 
    }

    void printy()
    {
        e++;
        cout<<"class a's private member "<<e<<endl; 
    }
protected:
    int b;
private:
    int e;

};

class B: public A{
public:
    void print()
    {
        cout<<b<<endl; //can assess the protected member directly.
        //cout<<e<<endl;
    }
};

int main()
{
    A oa(33,44);
    B ob;
    cout<<ob.a<<endl;
    
    oa.printx();
    oa.printy();
    ob.printy(); // why this is no compile error;
    ob.print();
    //cout<<oa.b<<endl; // b is a protected member, so it cannot be assessed by object.
    //cout<<ob.b<<endl;
    return 0;
}
#endif

#if 0 // when will the compiler create the default constructor ?
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

class A 
{
public:
    int a;
    string s;
    string s1;
    //string s1 = "hello world";

    void show()
    {
        cout<<a<<endl;
        cout<<s<<endl;
        cout<<s1<<endl;
    }
};

int main()
{
    A tmp;
    tmp.show();

    A tmp1 = {1, "fuck"};
    tmp1.show();

    // 4 forms to initalize 
    int i = 0;
    int j(1);
    int k{2};
    int l = {3};
    cout<<i<<" "<<j<<" "<<k<<" "<<l<<endl;

    vector<int> vi(10, 1); 
    for (auto iter = vi.begin(); iter != vi.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;

    int i1;
    int i2();
    cout<<i1<<" "<<i2<<endl;

    int *pi1 = new int;
    cout<<*pi1<<endl;
    int *pi2 = new int();
    cout<<*pi2<<endl;

    vector<string> v2{10,"hello"};
    vector<string> v3(10,"hello");
}
#endif
#if 0
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::string;

class A
{
    int a;
    string b;
#if 1
    A()
    {
        a = 1;
        b = "hello world";
        cout<<"constrct class A object"<<endl;
    }
#endif
public:
#if 1
    A(const class A &c)
    {
        a = c.a;
        b = c.b;
        cout<<"constrct class A object with copy constrct"<<endl;
    }
#endif
    A& operator=(const A &c)
    {
        a = c.a;
        b = c.b;
        cout<<"constrct class A object with operator="<<endl;
        return *this;
    }
    ~A()
    {
        a--;
        cout<<"disconstrct class A object"<<endl;
    }
    void test(const A &);
};

void A::test(const A &a)
{
    cout<<"class a "<<a.a<<" "<<a.b<<endl;
}

int main()
{
    A c;
    //vector<A> a;
    //a.push_back(c); //vector doesn't support insert, but push api xxxx no

    
    //A b = c;
    A b;
    b = c;
#if 0
    string tem("hello world");
    A::test(tem); //why can not use implicit comversion?
#endif
    return 0;
}
#endif

#if 0 //vector size and capacity
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;

int main()
{
    vector<int> str;
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;
    for (vector<int>::size_type ix = 0; ix != 24; ix++)
    {
        auto iter = str.end();
        str.insert(iter, ix); 
        //str.push_back(ix);
    }
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;
    cout<<&str<<endl;
    
    str.reserve(33); //set the capacity to the specified value
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;

    while (str.size() != str.capacity())
    {
        str.push_back(0);
    }
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;
    
    str.push_back(0);
    cout<<&str<<endl; //alloc memory again
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;

    str.shrink_to_fit(); //reduce capacity() to equal size()
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;

    str.clear();
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;
    return 0;
}
#endif

#if 0 // vector operations => push_back, insert, pop
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template<typename type>
void show(vector<type> &vt)
{
    if (!vt.empty())
    {
        for (auto i = vt.begin(); i != vt.end(); i++)
        {
            //cout<<vt[i]<<" ";
            //cout<<vt.at(i)<<" ";
            cout<<*i<<" ";
        }
    }
    else
        cout<<"empty vector.";
    cout<<endl;
}

int main()
{
    vector<int> vi;
    vi.push_back(1);
    //show<int>(vi);
    show(vi);
    // prototype: iterator insert(__position, __value)
    // prototype: void insert(__position, __initalize_list)
    // prototype: void insert(__position, __size, __value)

    auto iter = vi.begin();
    cout<<"iter: "<<*iter<<endl;
    iter = vi.insert(iter, 3);
    cout<<"iter: "<<*iter<<endl;
    show(vi);
   
    iter = vi.begin();
    vi.insert(iter, {7,7,7});
    cout<<"iter: "<<*iter<<endl;
    show(vi);
   
    iter = vi.begin(); // why need to get this iterator again?
    vi.insert(iter, 3, 8);
    cout<<"iter after insert: "<<*iter<<endl;
    show(vi);
   
    vi.pop_back();
    show(vi);
    iter = vi.begin(); // why need to get this iterator again?
    vi.erase(iter);
    show(vi);
    cout<<"before clear(): "<<vi.size()<<" "<<vi.capacity()<<endl;
    vi.clear();
    show(vi);

    //prealloc space for vector
    vector<int> v;
    cout<<v.size()<<" "<<v.capacity()<<endl;
    vector<int> v1(33,1);
    cout<<v1.size()<<" "<<v1.capacity()<<endl;
    vector<int> v2;
    v2.resize(33);  //append space and alloc new elements
    cout<<v2.size()<<" "<<v2.capacity()<<endl;
    v2.resize(24);
    cout<<v2.size()<<" "<<v2.capacity()<<endl;
    vector<int> v3;
    v3.reserve(33); //just append space
    cout<<v3.size()<<" "<<v3.capacity()<<endl;
    return 0;
}
#endif

#if 0 // for_each api
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
int main()
{
    string c("hello world");
    for_each(c.begin(), c.end(), [] (char &s) { cout<<s<<endl; });

    return 0;
}
#endif

#if 0 // pointer and reference
void test(int *p, int * &q)
{
    int i = 1;
    cout<<"&: "<<&i<<endl;
    cout<<"&p: "<<&p<<endl;
    cout<<"&q: "<<&q<<endl;
    //p = &i;;
    q = &i;;
}

void test2(int *p)
{
    int i = 2;
    p = NULL;
}

int main()
{
    int a = 10;
    int *p = &a;
    int * &q = p;
    cout<<"&p: "<<&p<<endl;
    cout<<"&q: "<<&q<<endl;
    cout<<"p: "<<p<<endl;
    cout<<"q: "<<q<<endl;
    test(p, q);
    cout<<"p: "<<p<<endl;
    cout<<"q: "<<q<<endl;

    test2(p);
    cout<<"after test, p "<<p<<endl;

    short bb = 1;
    short *pbb = &bb;
    short &aa = bb;
    cout<<sizeof(bb)<<endl;
    cout<<sizeof(pbb)<<endl;
    cout<<sizeof(p)<<endl;
    
    return 0;
}
#endif

#if 0
class A
{
    public:
        static int a;
        int *p;
    public:
        A()
        {
                a = 1;
                cout<<"create A"<<endl;
                p = new int(32);
        }

        ~A()
        {
                if (p)
                {
                        delete p;
                }
        }
};
int A::a = 10;

class B: public A
{
    public:
        B()
        {
                a++;
                cout<<"create B"<<endl;
        }

};

int main()
{
        B b;
        cout<<b.a<<endl;

        A a;
        A c(a); 
        return 0;
}
#endif

#if 0 // override vs hide
#include <iostream>
using std::cout;
using std::endl;

class A
{
    public:
        virtual void print()
        {
            cout<<"A: virtual function 1"<<endl;
        }
        void printx()
        {
            cout<<"A: function 2"<<endl;
        }
};

class B: public A
{
    public:
        void print() //override 
        {
            cout<<"B: virtual function 1"<<endl;
        }
        void printx() //hide
        {
            cout<<"B: function 2"<<endl;
        }
};

int main()
{
    A a;
    B b;

    a.print();
    a.printx();
    
    b.print();
    b.printx();
 
    A *q = &b;
    q->print();
    q->printx();
    return 0;
}
#endif

#if 0 // virtual function table memory layout
int main()
{ 
    void (*fun)(A*);
    A *p = new B;
    long lVptrAddr; 
   
    cout<<sizeof(A)<<endl;
    cout<<sizeof(B)<<endl;
    memcpy(&lVptrAddr,p,8);
    cout<<p<<endl;
    printf("%p\n", lVptrAddr);  // lVptrAddr is the address of virfun table
    printf("%p\n", *((long *)lVptrAddr));
    memcpy(&fun, reinterpret_cast<long*>(lVptrAddr),8);
    printf("%p\n", fun);
    fun(p); 
    memcpy(&fun, reinterpret_cast<long*>(lVptrAddr+8),8);
    printf("%p\n", fun);
    fun(p); 

    delete p; 
    //system("pause"); 
} 
#endif

#if 0
//direct initialize and copy initialize

#include <iostream>
#include <string.h>

using std::cin;
using std::cout;
using std::endl;

class ClassTest
{
    public:
        ClassTest()
        {
            c[0] = '\0';
            cout<<"ClassTest()"<<endl;
        }

        ClassTest& operator=(const ClassTest &ct)
        {
            strcpy(c, ct.c);
            cout<<"ClassTest& operator=(const ClassTest &ct)"<<endl;
            return *this;
        }
        
        //explicit ClassTest(const char *pc)
        ClassTest(const char *pc)
        {
            strcpy(c, pc);
            cout<<"ClassTest (const char *pc)"<<endl;
        }
    //private:
        ClassTest(const ClassTest& ct)
        {
            strcpy(c, ct.c);
            cout<<"ClassTest(const ClassTest& ct)"<<endl;
        }

    private:
        char c[256];
};

int main()
{
    cout<<"ct1: ";
    ClassTest ct1("ab");
    cout<<"ct2: ";
    ClassTest ct2 = "ab"; //converting constructor
    cout<<"ct3: ";
    ClassTest ct3 = ct1; //  why not use = constructor
    cout<<"ct4: ";
    ClassTest ct4(ct1);
    //cout<<"ct5: ";
    //ClassTest ct5 = ClassTest();
    return 0;
}
#endif

#if 0
#include <iostream>

using std::cout;
using std::endl;
class CExample 
{
    private:
        int a;

    public:

        CExample(int b)
        { 
            a = b;
            cout<<"creat constructor: "<<a<<endl;
        }


        CExample(const CExample& C) // why add const
        {
            a = C.a;
            cout<<"copy constructor: "<<a<<endl;
        }

        ~CExample()
        {
            cout<< "delete: "<<a<<endl;
        }

        void Show ()
        {
            cout<<a<<endl;
        }
};

void g_Fun(CExample C)
{
    cout<<"test"<<endl;
}

int main()
{
    CExample test(1);

    g_Fun(test);

    return 0;
}
#endif

#if 0
#include <iostream>
using std::cout;
using std::endl;

class Rect
{
    public:
        Rect()      
        {
            count++;
            p = new int(1);
            cout<<"add default"<<endl; // default constructor 
        }
#if 1
        Rect(const Rect & rec)   //why add const   
        {
            width = rec.width;
            height = rec.height;
            p = new int(1);
            count++;
            cout<<"add copy"<<endl; // copy constructor 
        }
#endif
        ~Rect()     
        {
            count--;
            if (p)
                delete p;
            cout<<"del"<<endl;
        }

        static int getCount()       
        {
            return count;
        }

        void show()
        {
            cout<<p<<endl;
        }

    private:
        int width;
        int height;
        int *p;
        static int count;       
};

int Rect::count = 0;        

int main()
{
    Rect rect1;
    cout<<"The count of Rect: "<<Rect::getCount()<<endl;
    rect1.show();

    Rect rect2(rect1);   
    cout<<"The count of Rect: "<<Rect::getCount()<<endl;
    rect2.show();

    Rect rect3 = rect1;   
    cout<<"The count of Rect: "<<Rect::getCount()<<endl;
    rect3.show();
    return 0;
}
#endif
