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

// 显示信息函数
extern void show_info(int len, struct book_info *show);

// 修改图书信息函数
void change_info(int len, struct book_info *book)
{
    int num, i;
    printf("请输入要修改的信息的序号: ");
    scanf("%d", &num);
    while (getchar() != '\n')
        ;
    // 防止输入的序号过大或过小
    if (num < 1 || num > len)
    {
        printf("error\n");
        change_info(len, book);
    };
    num = num - 1;
    for (i = 0; i < len; i++)
    {
        if (book[num].num == 0)
        {
            printf("该序号下无信息!\n");
            return;
        }
    }

    printf("书名: ");
    scanf("%s", book[num].title);
    while (getchar() != '\n')
        ;

    printf("书名: ");
    scanf("%s", book[num].author);
    while (getchar() != '\n')
        ;

    printf("页数: ");
    scanf("%d", &book[num].pages);
    while (getchar() != '\n')
        ;

    printf("价格: ");
    scanf("%f", &book[num].price);
    while (getchar() != '\n')
        ;

    printf("图书(%d)信息修改成功！\n", book[num].num);
    show_info(5, book);
}