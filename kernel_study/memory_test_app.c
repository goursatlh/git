#include <stdio.h>
#include <stdlib.h>
static int global_Var = 1;
static int mxpmem_dev_id = -1;
#define MXP_MEM_DEVICE_NAME "timxpmem"
#define MXP_MEM_DEVICE_FILE_NAME "/dev/"MXP_MEM_DEVICE_NAME

#define MXP_MEM_BLK_ALLOC 1

struct TEST_MSG {
    void *data;
};

int main()
{
    struct TEST_MSG msg;
    char cmd[128] = {0};
    msg.data = &global_Var;
    
    sprintf(cmd, "insmod memory_test.ko");
    system(cmd);
    sleep(3);
    printf("global_Var %p, before modify %d\n", &global_Var, global_Var);
    if (!(mxpmem_dev_id = open(MXP_MEM_DEVICE_FILE_NAME, 0)))
    {
        printf("Error opening %s\n", MXP_MEM_DEVICE_FILE_NAME);
        return 0;
    }

    ioctl(mxpmem_dev_id, MXP_MEM_BLK_ALLOC, &msg);
    sleep(3);
    printf("global_Var %p, after modify %d\n", &global_Var, global_Var);

    if (mxpmem_dev_id)
        close(mxpmem_dev_id);
    
    sprintf(cmd, "rmmod memory_test");
    system(cmd);
    return 0;
}
