#ifndef _TALK_H_
#define _TALK_H_
// 头文件
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// 宏定义
#define J2R 10
#define R2J 20

// 结构体
typedef struct my_buf
{
    long mtype;     //类型
    char mtext[50]; //正文
} talk_buf;

#endif