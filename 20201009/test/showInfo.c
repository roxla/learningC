#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 声明一个图书信息结构体
struct book_info
{
    int num;         //序号
    char title[20];  //书名
    char author[20]; //作者名
    int pages;       //页数
    float price;     //价格
};

// 显示图书信息函数
// void show_info(int len, struct book_info show[])
// void show_info(int len, struct book_info show[len])
void show_info(int len, struct book_info *show)
{
    system("clear"); //调用 clear 指令，清屏
    printf("==============INFO=================\n");
    printf("序号\t 书名\t 作者名\t 页数\t 价格\n");

    int i;
    for (i = 0; i < len; i++)
    {
        if (show[i].num == 0)
            break;
        printf("%d\t %s\t %s\t %d\t %.3f\n",
               show[i].num,
               show[i].title,
               show[i].author,
               show[i].pages,
               show[i].price);
    }

    // (show+i)->num,
    // (show+i)->title,
    // (show+i)->pages,
    // (show+i)->price);
    // 结构体指针->成员名
    // 结构体变量.成员名
}