#ifndef _MY_H_ //如果没有引入头文件my.h
#define _MY_H_ //那就引入头文件my.h

/************** 头文件 ***************/
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "list.h"

/*************** 宏定义 ******************/
#define ROW 10         // 被分割字符串的行长度
#define COL 25         // 被分割字符串的列长度
#define SIZE_W 800     // 屏幕高度
#define SIZE_H 480     // 屏幕高度
#define PIC_PATH "pic" // 图片存储目录
#define PIC_TYPE "ebm" // 图片文件后缀

/************* 结构体声明 ****************/
typedef struct ebmlist
{
    // 数据域
    char name[257];      // 名字
    char path[257];      // 路径
    char path_name[514]; // 名字+路径
    long size;           // 大小
    long date;           // 时间戳
    long name_size;      //名字大小

    // 指针域（俗称：小结构体）
    struct list_head list; //（前后关系）
} ebm_st, *ebm_pt;

/*************** 全局变量 ***************/
int lcd_fd;       // LCD文件描述符
unsigned int *FB; // 显存首地址
int ts_fd;        //触摸屏文件描述符

/*************** 外部函数声明 ******************/
// 以某个字符串为分割符分割字符串，并返回最后一个字符串
extern char *cutOut(char str[], const char *delim, char **catalog);

/*************** LCD操作函数声明 ***************/
// 打开LCD
extern int open_lcd(void);
// 关闭LCD
extern void close_lcd(void);
// 显示全屏ebm图片
extern int show_pic_full(const char *pathname);

/*************** 触摸屏操作函数声明 ***************/
// 获取xy坐标(通过地址传递，修改两个变量数据)
extern void get_xy(int *x, int *y);
// 打开触摸屏
extern int open_ts(void);
// 关闭触摸屏
extern void close_ts(void);
// 获取xy坐标(通过地址传递，修改两个变量数据)
extern void get_xy(int *x, int *y);
// 获取xy坐标，滑动
extern void get_xy_slide();

/*************** 内核链表操作 ***************/
// 初始化内核链表
extern ebm_pt ebm_list_init(void);
// 添加数据到链表尾
extern int ebm_list_add_tail(char *name, char *path, char *path_name, long size, long date, long name_size, ebm_pt head);
// 打印链表所有数据（后序遍历）
extern void ebm_list_show(ebm_pt head);

/*************** 获取图片列表 ***************/
extern ebm_pt get_pic_list();

/***************** 链表排序 *****************/
extern void rearrange(int mode, const char *type, ebm_pt head);

#endif