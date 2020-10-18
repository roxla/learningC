#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h> //输入子系统头文件
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ROW 10
#define COL 25
#define PIC_TYPE "ebm" // 图片文件后缀
#define PIC_NUM 20     // 图片列表长度
#define PIC_PATH "pic"

void get_pic_list(char (*list)[256]);

int main(int argc, char const *argv[])
{
    char(*pic_list)[strlen(PIC_PATH) + 256] = calloc(PIC_NUM, strlen(PIC_PATH) + 256);
    if (pic_list == NULL)
    {
        perror("callo failed");
        return 1;
    }
    bzero(pic_list, sizeof(pic_list[0]));
    get_pic_list(pic_list);
    printf("%ld\n", sizeof(pic_list[0]));

    return 0;
}

void get_pic_list(char (*list)[256])
{
    strncpy(list[0], "name1", sizeof(list[0]));
    strncpy(list[1], "name2", sizeof(list[1]));
    strncpy(list[2], "name3", sizeof(list[2]));
}
