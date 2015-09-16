#if 1
#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::allocator;

int main()
{
    int *p = new int[100]{1,2,3,4};
    delete [] p;

    allocator<int> al;
    auto p1 = al.allocate(10); //only alloc memory, not construct
    al.construct(p1, 1234);
    cout<<p1<<" "<<*p1<<endl;
    al.construct(++p1, 5678);
    cout<<p1<<" "<<*p1<<endl;

    al.destroy(p1);
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

#if 0 //smart pointer
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;

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
    shared_ptr<string> p2; //p2 is 0 if it has not pointer to a object
    cout<<*p1<<endl;
    cout<<p1.get()<<endl;
    cout<<p1<<endl;
    cout<<p2<<endl;
    auto q(p1);
    cout<<*q<<endl;
    cout<<p1.use_count()<<endl;
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
#include <string>

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

int main()
{
    ifstream in("test");
    ofstream out("output");
    string s;
    while (getline(in, s))
    {
        cout<<s<<endl;
        out<<s<<endl;
    }
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
#if 0 // template class for a link
template <class Type> class Queue; 
template <class Type> class QueueItem { 
    friend class Queue<Type>;
    QueueItem(const Type &t): item(t), next(0) { }  
    Type item;      
    QueueItem *next;      
};

template <class Type> class Queue {  
    public:  
        Queue(): head(0), tail(0) { } 
        void push(const Type &);        
        void pop();                      
        bool empty() const 
        {            
            return (head == 0);  
        }
        void print_size()
        {
            cout<<"link note number: "<<size<<endl;
        }
        //void init_size(unsigned val){size = val;}

    private:  
        QueueItem<Type> *head;           
        QueueItem<Type> *tail;    
        static unsigned size;
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
    oa.print_size();
    Queue<string> ob;
    string s("hello");
    ob.push(s);

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
#if 0 // template
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
    cout<<compare(s3, s4)<<endl;
    cout<<compare(a, b)<<endl;
    cout<<comp("hello", "world")<<endl;
    cout<<comp(s4, s3)<<endl;
    //cout<<comp(s1, s2)<<endl;   // error, string isn't equal to char a[]
    //cout<<compare_ref(s1, s2)<<endl;
    cout<<compare_ref(s3, s4)<<endl;  //error, char [12] not equal to char [14]
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
class Base {
    public:
        void print() {
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

int main() {
    Derived d;
    d.print();
    d.print(10);
    d.print("");
    //d.Base::print(10);
    //d.Base::print("");
    return 0;
}
#endif

#if 0 //function overload 2
class Base{
    public:
        int display(int n_count){
            cout <<"display function defined in the Class Base"<<endl;
            return 0;
        }
};

class Derived: public Base{
    public:
        using Base::display;
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
    p_derived->display(8);
    p_derived->display(10,20);

    return 0;
}
#endif

#if 0 //function overload

void print(char i)
{
    cout<<i<<endl;
    return;
}

void print(long i)
{
    cout<<i<<endl;
    return;
}

int main()
{
    print(static_cast<long>(38));
    //print(38); //error
    return 0;
}

#endif

#if 0  // destructor should be virtual
class A
{
    public:
        A();
        //virtual ~A();
        ~A();
};

A::A()
{
}

A::~A()
{
    printf("Delete class APn\n");
}

class B : public A
{
    public:
        B();
        ~B();
};

B::B()
{ }

B::~B()
{
    printf("Delete class BPn\n");
}

int main(int argc, char* argv[])
{
    A *b=new B;
    delete b;
    return 0;
}
#endif

#if 0 //defalut constrctor and public/protected inherit
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
        cout<<b<<endl;
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

#if 0
class A
{
public:
    int a;
    A()
    {
        a = 1;
        cout<<"constrct class A object"<<endl;
    }
    A(const class A &c)
    {
        //this->a = a.a;
        a = c.a;
        cout<<"constrct class A object with copy constrct"<<endl;
    }
    ~A()
    {
        a--;
        cout<<"disconstrct class A object"<<endl;
    }
};

void test(class A a)
//void test(class A &a)
{
    cout<<"class a "<<a.a<<endl;
}

int main()
{
    A c;
    test(c);
    cout<<c.a<<endl;
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
    
    str.reserve(33); //set the capacity to the specified
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;

    while (str.size() != str.capacity())
    {
        str.push_back(0);
    }
    str.push_back(0);
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;

    str.shrink_to_fit();
    cout<<str.size()<<endl;
    cout<<str.capacity()<<endl;
    
    return 0;
}
#endif

#if 0 // iterator
int main()
{
    string c("hello world");
    iterator j;
    if (c.begin() != c.end())
    {

        auto i = c.begin();
        cout<<i<<endl;
        i++;
    }

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

#if 0 // virtual function 
class A
{
    public:
        virtual void print()
        {
            cout<<"A: virtual function 1"<<endl;
        }
        virtual void printx()
        {
            cout<<"A: virtual function 2"<<endl;
        }
};

class B: public A
{
    public:
        //void print() override
        //using A::print;
        void print()
        {
            cout<<"B: virtual function 1"<<endl;
        }
        void printx()
        {
            cout<<"B: virtual function 2"<<endl;
        }
};
#endif
#if 0
int main()
{
    A a;
    B b;

    a.print();
    b.print();
    A *p = &a;
    A *q = &b;

    p->print();
    q->print();
    //n->print();  // pointer must be initialized before accessed.
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
    ClassTest ct3 = ct1;
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
            cout<<"creat: "<<a<<endl;
        }


        CExample(const CExample& C)
        {
            a = C.a;
            cout<<"copy"<<endl;
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
            cout<<"add default"<<endl;
        }
#if 1
        Rect(const Rect & rec)   //why add const   
        {
            width = rec.width;
            height = rec.height;
            p = new int(1);
            count++;
            cout<<"add copy"<<endl;
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
