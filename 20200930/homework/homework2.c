#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    int i;
    char command[60];
    // strncpy(command, "echo `date +\"%Y-%m-%d %H:%M:%S\"` >> time.txt", sizeof(command));
    strncpy(command, "date +\"%Y-%m-%d %H:%M:%S\" >> time.txt", sizeof(command));
    for (i = 0; i < 11; i++)
    {
        sleep(1); //linux 中 sleep 函数以秒为单位
        system(command);
    }
    system("echo ======================== >> time.txt");
    printf("输入完成\n");

    return 0;
}

// echo `date +"%Y-%m-%d %H:%M:%S"`