#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

void print_info(
        char*      var_name,
        char*      var_ptr,
        size_t     size_in_kb
)
{
    printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n",
                               var_name, size_in_kb, var_ptr, sbrk(0));
}

int main(int argc, char** argv)
{
        char *heap_var1, *heap_var2, *heap_var3 ;
        char *mmap_var1, *mmap_var2, *mmap_var3 ;
        char *maybe_mmap_var;
        printf("Orginal heap top is 0x%lx\n", sbrk(0));
        heap_var1 = malloc(32*1024);
        print_info("heap_var1", heap_var1, 32);
        heap_var2 = malloc(64*1024);
        print_info("heap_var2", heap_var2, 64);
        heap_var3 = malloc(127*1024);
        print_info("heap_var3", heap_var3, 127);
        printf("\n");
        maybe_mmap_var = malloc(128*1024);
        print_info("maybe_mmap_var", maybe_mmap_var, 128);
        //mmap
        mmap_var1 = malloc(128*1024);
        print_info("mmap_var1", mmap_var1, 128);
        // set M_MMAP_THRESHOLD to 64k
        mallopt(M_MMAP_THRESHOLD, 64*1024);
        printf("set M_MMAP_THRESHOLD to 64k\n");
        mmap_var2 = malloc(64*1024);
        print_info("mmap_var2", mmap_var2, 64);
        mmap_var3 = malloc(127*1024);
        print_info("mmap_var3", mmap_var3, 127);
        return 1;
}
#endif


#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>
char ps_cmd[1024];
void print_info(
        char*      var_name,
        char*      var_ptr,
        size_t     size_in_kb
)

{
        printf("Address of %s(%luk) 0x%lx,  now heap top is 0x%lx\n",
                 var_name, size_in_kb, var_ptr, sbrk(0));
        system(ps_cmd);
}

int main(int argc, char** argv)
{
        char *non_set_var, *set_1k_var, *set_5k_var, *set_7k_var;
        pid_t pid;
        pid = getpid();
        sprintf(ps_cmd, "ps aux | grep %lu | grep -v grep|awk '{print \"VSZ: \"$5\" RSS: \" $6}'", pid);
        non_set_var = malloc(32*1024);
        print_info("non_set_var", non_set_var, 32);
        set_1k_var = malloc(64*1024);
        memset(set_1k_var, 0, 1024);
        print_info("set_1k_var", set_1k_var, 64);
        set_5k_var = malloc(127*1024);
        memset(set_5k_var, 0, 5*1024);
        print_info("set_5k_var", set_5k_var, 127);
        set_7k_var = malloc(64*1024);
        memset(set_1k_var, 0, 7*1024);
        print_info("set_7k_var", set_7k_var, 64);
        return 1;
}
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

size_t  heap_malloc_total, heap_free_total, mmap_total, mmap_count;

void print_info()
{
        struct mallinfo mi = mallinfo();
        printf("count by itself:\n");
        printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu\n",
                heap_malloc_total*1024, heap_free_total*1024, heap_malloc_total*1024 - heap_free_total*1024,
                mmap_total*1024, mmap_count);
        printf("count by mallinfo:\n");
        printf("\theap_malloc_total=%lu heap_free_total=%lu heap_in_use=%lu\n\tmmap_total=%lu mmap_count=%lu, %lu, %lu, %lu\n",
                mi.arena, mi.fordblks, mi.uordblks,
                mi.hblkhd, mi.hblks, mi.fsmblks, mi.smblks, mi.ordblks);
        printf("from malloc_stats:\n");
        malloc_stats();
}

#define ARRAY_SIZE 200
int main(int argc, char** argv)
{
        char** ptr_arr[ARRAY_SIZE];
        int i;
        for( i = 0; i < ARRAY_SIZE; i++) {
                ptr_arr[i] = malloc(i * 1024);
                if ( i < 128)
                        heap_malloc_total += i;
                else {
                        mmap_total += i;
                        mmap_count++;
                }

        }
        print_info();
        for( i = 0; i < ARRAY_SIZE; i++) {
                if ( i % 2 == 0)
                        continue;
                free(ptr_arr[i]);
                if ( i < 128)
                        heap_free_total += i;
                else {
                        mmap_total -= i;
                        mmap_count--;
                }
        }
        printf("\nafter free\n");
        print_info();
        return 1;
}


