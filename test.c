#if 1 // array override
/*--------
 *argument3 c       
 *argument2 b
 *argument1 a
 *next instruction address
 *ebp for main                        4 
 *ret                                 4
 *sum                                 4
 *buffer                              14
 * */
#include <stdio.h>
int function(int a, int b, int c)
{
    char buffer[14];          
    int sum;                  
    int *ret;

    ret = buffer + 26;
    (*ret) += 8;
    sum = a + b + c;
    return sum;
}

void main()
{
    int x;
    x = 0;
    function(1,2,3);
    x = 1;

    printf("%d\n",x);
}
#endif

#if 0 // function call flow
#include <stdio.h>
void func(int x, int y)
{
    printf("%p: x\n", &x);
    printf("%p: y\n", &y);
    int c = x + y;
    printf("%p: c\n", &c);
}
void main()
{
    int a = 1;
    int b = 2;

    printf("%p: a\n", &a);
    printf("%p: b\n", &b);

    func(a, b);
}
#endif


#if 0  // backtrace() to dump the call stack
#include <stdio.h>
#include <execinfo.h>
#include <dlfcn.h>

void **getEBP( int dummy )
{
    void **ebp = (void **)&dummy -2 ;
    return( *ebp );
}
void print_walk_backtrace( void )
{
    int dummy;
    int frame = 0;
    Dl_info dlip;
    void **ebp = getEBP( dummy );
    void **ret = NULL;
    printf( "Stack backtrace:\n" );
    while( ebp )
    {
        ret = ebp + 1;
        dladdr( *ret, &dlip );
        printf("Frame %d: [ebp=0x%08x] [ret=0x%08x] %s\n",
                frame++, *ebp, *ret, dlip.dli_sname );
        ebp = (void**)(*ebp);
        /* get the next frame pointer */
    }
    printf("---------------------------------------------------------\n");
}

void do_gnu_backtrace()
{
#define BACKTRACE_SIZ 100
    void *array[BACKTRACE_SIZ];
    size_t size, i;
    char **strings;

    size = backtrace(array, BACKTRACE_SIZ);
    strings = backtrace_symbols(array, size);

    printf("-------------------------------begin to dump the call stack ---------------------------------\n");
    for (i = 0; i < size; ++i) {
        //printf("%p : %s\n", array[i], strings[i]);
        printf("%s\n", strings[i]);
    }

    printf("---------------------------------------------------------------------------------------------\n");
    printf("int sizeof %d\n", sizeof(int));
    printf("int sizeof %d\n", sizeof(int *));
    free(strings);
}

void func1()
{
    printf("fuck you then.\n");
    do_gnu_backtrace();
    //print_walk_backtrace();
}
void func()
{
    printf("hello world.\n");
    func1();
    //do_gnu_backtrace();
    //print_walk_backtrace();
}

void main()
{
   func(); 
}
#endif
