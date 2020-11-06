#include "server.h"

int fifo_fd;

void server(int sig);

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
    fifo_fd = open("/home/roxlas/fifo_test", O_RDWR);
    if (fifo_fd < 0)
        printf("open error!\n");

    // 发送自身的pid号
    char buf[50] = {0};
    snprintf(buf, sizeof(buf), "%d", getpid());
    write(fifo_fd, buf, strlen(buf)); //将pid的数据写入到管道文件中。
    // 等待服务器就绪
    printf("等待服务器就绪...\n");
    signal(SIGUSR1, server);
    pause();

    // 关闭文件
    close(fifo_fd);

    return 0;
}

void server(int sig)
{
    printf("服务器已就绪\n");
    // 进程扇
    int i, state;
    pid_t x;
    int n;
    printf("pls input child process num:");
    scanf("%d", &n); //5
    while (getchar() != '\n')
        ;
    for (i = 0; i < n; i++) // 生多少个小孩，就循环多少次。
    {
        x = fork(); // 父子进程做的事情不一样，就要通过返回值来判断。
        if (x > 0)  // 父进程
        {
            sleep(1);
            continue;
        }

        if (x == 0) // 子进程
            break;
    }
    if (x > 0)
    {
        char buf[] = "end";
        // 回收所有子进程的资源
        for (i = 0; i < n; i++)
            wait(&state);
        write(fifo_fd, buf, strlen(buf)); //将pid的数据写入到管道文件中。
    }

    if (x == 0)
    {
        char buf[50];
        snprintf(buf, sizeof(buf), "%d", getpid());
        printf("pid: %d\n", getpid());
        write(fifo_fd, buf, strlen(buf)); //将pid的数据写入到管道文件中。
        exit(0);
    }
}