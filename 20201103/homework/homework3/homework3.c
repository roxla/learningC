#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern void showPic();

int main()
{
    //1. 申请数组
    int fd[2] = {0};

    //2. 初始化无名管道
    pipe(fd);

    //3. 带着这条无名管道，去产生一个子进程
    pid_t x;
    x = fork();
    if (x > 0) //父进程
    {
        int state;
        char buf[20] = {0};
        wait(&state);
        if (state == 0)
        {
            read(fd[0], buf, sizeof(buf));
            if (!strcmp(buf, "show_bmp"))
                showPic();
        }
        else
        {
            printf("error!\n");
        }
        exit(0);
    }

    if (x == 0) //子进程
    {
        char buf[20] = {0};
        printf("Pls input:\n");
        fgets(buf, sizeof(buf), stdin);
        strtok(buf, "\n");
        while (getchar() != '\n')
            ;
        write(fd[1], buf, strlen(buf));
        exit(0);
    }

    return 0;
}