#include <stdio.h>
#include <execinfo.h>
#include <dlfcn.h>

typedef struct
{
      __const char *dli_fname;      /* File name of defining object.  */
        void *dli_fbase;              /* Load address of that object.  */
          __const char *dli_sname;      /* Name of nearest symbol.  */
            void *dli_saddr;              /* Exact value of nearest symbol.  */
} Dl_info;
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

    for (i = 0; i < size; ++i) {
        printf("%p : %s\n", array[i], strings[i]);
    }

    printf("---------------------------------------------------------\n");
    free(strings);
}

void func()
{
    printf("hello world.\n");
    do_gnu_backtrace();
    print_walk_backtrace();
}

void main()
{
   func(); 
}
