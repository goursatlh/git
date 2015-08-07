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

#if 1 // template
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
    cout<<compare(a, b)<<endl;
    cout<<comp("hello", "world")<<endl;
    cout<<comp(s4, s3)<<endl;
    //cout<<comp(s1, s2)<<endl;   // error, string isn't equal to char a[]
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
