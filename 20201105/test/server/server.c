#include "server.h"

int main(int argc, char const *argv[])
{
    // 创建一个有名管道
    int ret;
    if (access("/home/roxlas/fifo_test", F_OK) == -1)
    {
        ret = mkfifo("/home/roxlas/fifo_test", 0751);
        if (ret < 0)
            printf("mkfifo error!\n");
    }
    // 打开管道文件
    FILE *fifo_fd
        fifo_fd = fopen("/home/roxlas/fifo_test", "r+");
    if (fp == NULL)
        perror("fopen failed");

    // 发送就绪信号给客户端
    char r_buf[50] = {0};
    fgets(r_buf, sizeof(r_buf), fifo_fd);
    printf("%s", r_buf);
    // pid_t x;
    // while (1)
    // {
    //     bzero(r_buf, sizeof(r_buf));
    //     read(fifo_fd, &r_buf, sizeof(r_buf));
    //     if (!strcmp(r_buf, "end"))
    //         break;
    //     else if ()
    //     {
    //         x = atoi(r_buf);
    //         kill(x, SIGUSR1);
    //     }
    //     else if(strlen(r_buf))
    //         printf("from Jack:%s\n", r_buf);
    // }

    // 关闭文件
    close(fifo_fd);

    return 0;
}
