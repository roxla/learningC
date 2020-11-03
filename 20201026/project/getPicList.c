#include <dirent.h>

#include "my.h"

/*************** 全局变量 *******************/
char *path_name;
/*************** 函数声明 ******************/
// 文件类型检索
int serch_pic_file();
// 打开图片文件夹
int open_pic_file();
// 添加文件数据到链表
int add_pic_file(char *path, char *name, ebm_pt head);

// 入口函数(通过地址传长度)
ebm_pt get_pic_list(int *length)
{
    // 用于存储路径+文件名
    path_name = malloc(514);
    // 初始化内核链表
    ebm_pt pic_list = ebm_list_init();
    // 打开图片文件夹
    open_pic_file(pic_list);
    // 释放堆空间
    free(path_name);
    // 将链表头返回
    return pic_list;
}

// 打开图片文件夹
int open_pic_file(ebm_pt head)
{
    // 打开目录
    DIR *dp;
    dp = opendir(PIC_PATH);
    if (dp == NULL)
    {
        perror("open dir failed");
        return 1;
    }
    // 读取目录项
    struct dirent *temp = NULL;
    while (1)
    {
        temp = readdir(dp);
        // 判断是否读取完毕
        if (temp == NULL)
            break;
        // 去除 "." 和 ".."
        // if(temp->d_name=="." || temp->d_name=="..")	//不允许比较！
        // if(strcmp(temp->d_name, ".")==0 || strcmp(temp->d_name, "..")==0)
        if (!strcmp(temp->d_name, ".") || !strcmp(temp->d_name, ".."))
            continue;
        // 筛选文件类型
        if (serch_pic_file(temp->d_name))
            // 将文件数据装入链表
            add_pic_file(PIC_PATH, temp->d_name, head);
    }
    // 关闭目录
    closedir(dp);

    return 0;
}

// 添加文件数据到链表
int add_pic_file(char *path, char *name, ebm_pt head)
{
    bzero(path_name, 514);
    snprintf(path_name, 514, "%s/%s", path, name);
    // 获取名字长度
    char name_size[257];
    snprintf(name_size, 257, "%s", name);
    int i;
    long sum = 0;
    for (i = 0; i < strlen(name_size); i++)
        sum += name_size[i];
    // 获取属性
    struct stat buf;
    if (stat(path_name, &buf) == -1)
    {
        perror("stat failed");
        return 1;
    }
    ebm_list_add_tail(name, path, path_name, buf.st_size, buf.st_mtime, sum, head);

    return 0;
}

// 文件类型检索
int serch_pic_file(char *name)
{
    // 变量定义
    char temp[256];
    char *catalog;
    // 比较文件后缀，相同返回1，不同返回0
    strncpy(temp, name, sizeof(temp));
    cutOut(temp, ".", &catalog);
    if (!strcmp(catalog, PIC_TYPE))
        return 1;
    else
        return 0;
}
