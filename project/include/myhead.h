#ifndef _MYHEAD_H_
#define _MYHEAD_H_

/************** 头文件 ***************/
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include "thread_pool.h"

/*************** 宏定义 ******************/
#define ROW 10         // 被分割字符串的行长度
#define COL 25         // 被分割字符串的列长度

/*************** 全局变量 ***************/
// 命名前缀
extern char name_head[50];
// 文件后缀
extern char name_type[10];
// 线程池指针
extern thread_pool *pool;

/*************** 外部函数声明 ******************/
// 重命名文件入口函数
extern void rename_file(char const *src_path);
// 以某个字符串为分割符分割字符串，并返回最后一个字符串
extern char *cutOut(char str[], const char *delim, char **catalog);

#endif