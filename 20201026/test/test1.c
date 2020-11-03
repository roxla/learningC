#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void check_file(char *name);
int get_pic_list();

int main(int argc, char const *argv[])
{
    get_pic_list();
    return 0;
}

// 打开图片文件夹
int get_pic_list()
{
    /*************** 变量定义 ***************/
    int i = 0;
    bool pic_file = false;
    char *name = malloc(256);
    /*************** 目录操作函数 ***************/
    DIR *dp = NULL;
    dp = opendir("./");
    if (dp == NULL)
    {
        perror("stat failed");
        return 0;
    }
    struct dirent *ep = NULL;
    while (1)
    {
        ep = readdir(dp);
        if (ep == NULL)
            break;
        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;
        printf("%s\n", ep->d_name);
    }

    // 如果没有文件，则打印
    if (!pic_file)
    {
        printf("File not found\n");
    };
    /*************** 关闭文件夹  ***************/
    closedir(dp);

    return 0;
}

// 检查文件类型
void check_file(char *name)
{
    struct stat buf;
    // 获取文件类型
    if (stat(name, &buf) == -1)
    {
        perror("stat failed");
        return;
    }
    // 判断文件类型
    if ((buf.st_mode & S_IFMT) == S_IFDIR)
    {
        // 目录类型：打开
        get_pic_list();
    }
    else if ((buf.st_mode & S_IFMT) == S_IFREG)
    {
        // 普通文件类型：筛选
        printf("%s\n", name);
    }
}