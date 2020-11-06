#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    int ret;
    ret = mkfifo("/home/ubuntu/fifo_test", 0751);
    if (ret == 0)
    {
        printf("mkfifo success!\n");
    }
    else
    {
        printf("mkfifo error!\n");
    }

    return 0;
}
