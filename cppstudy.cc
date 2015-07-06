#include <iostream>
#include <unistd.h>

using namespace std;

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

#if 1 
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
