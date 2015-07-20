#include <iostream>
#include <string>
#include <iterator>
#include <unistd.h>
#include <vector>
//using namespace std; // don't use this style code, that may cause name conflict
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

#if 1 //defalut constrctor and public/protected inherit
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

#if 0 //vector push_back
int main()
{
    vector<string> str(1);
    str[0] = "hello world";
    cout<<str[0]<<endl;

    vector<string> strnull;
    cout<<"strnull size "<<strnull.size()<<endl;
    strnull.push_back("hello world");
    cout<<"strnull size "<<strnull.size()<<endl;
    cout<<strnull[0]<<endl;

    vector<string>::iterator iter = strnull.begin();
    cout<<*iter<<endl;

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

#if 0 
class A
{
    public:
        virtual void print()
        {
            cout<<"hello A"<<endl;
        }
};

class B: public A
{
    public:
        void print()
        {
            cout<<"hello B"<<endl;
        }
};

int main()
{
    A a;
    B b;
    char cc[] = "hello world\n";
    string s(10, 'f');

    cout<<cc;
    cout<<s<<endl;
    cout<<sizeof(a)<<endl;
    cout<<sizeof(b)<<endl;

    a.print();
    b.print();

    A *p = &a;
    A *q = &b;

    p->print();
    q->print();

    return 0;
}
#endif

#if 0
int main()
{ 
    void (*fun)(A*);
    A *p = new B;
    long lVptrAddr; 
    
    printf("%s/%d: enter\n", __FUNCTION__, __LINE__);
    memcpy(&lVptrAddr,p,4); 
    printf("%s/%d: enter %x/%x\n", __FUNCTION__, __LINE__, lVptrAddr, &fun);
    sleep(10000000); 
    memcpy(&fun, reinterpret_cast<long*>(lVptrAddr),4);
    printf("%s/%d: enter\n", __FUNCTION__, __LINE__);
    fun(p); 

    printf("%s/%d: enter\n", __FUNCTION__, __LINE__);
    delete p; 
    system("pause"); 
} 
#endif

#if 0
//direct initialize and copy initialize
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
    ClassTest ct2 = "ab";
    cout<<"ct3: ";
    ClassTest ct3 = ct1;
    cout<<"ct4: ";
    ClassTest ct4(ct1);
    cout<<"ct5: ";
    ClassTest ct5 = ClassTest();
    return 0;
}
#endif

#if 0
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
class Rect
{
    public:
        Rect()      
        {
            count++;
            cout<<"add"<<endl;
        }
#if 0
        Rect(const Rect & rec)   //why add const   
        {
            width = rec.width;
            height = rec.height;
            count++;
            cout<<"add"<<endl;
        }
#endif
        ~Rect()     
        {
            count--;
            cout<<"del"<<endl;
        }
        static int getCount()       
        {
            return count;
        }
    private:
        int width;
        int height;
        static int count;       
};

int Rect::count = 0;        

int main()
{
    Rect rect1;
    cout<<"The count of Rect: "<<Rect::getCount()<<endl;

    Rect rect2(rect1);   
    cout<<"The count of Rect: "<<Rect::getCount()<<endl;

    return 0;
}
#endif
