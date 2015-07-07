#include <iostream>
#include <unistd.h>

//using namespace std; // don't use this style code, that may cause name conflict
using std::cin;
using std::cout;
using std::endl;
using std::string;

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
