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

// 删除图书信息函数
void delete_info(int len, struct book_info *book)
{
    int num, i;
    printf("请输入要删除的信息的序号: ");
    scanf("%d", &num);
    while (getchar() != '\n')
        ;
    // 防止输入的序号过大或过小
    if (num < 1 || num > len)
    {
        printf("error\n");
        delete_info(len, book);
    };
    // 删除该序号对应的信息
    for (i = num - 1; i < len; i++)
    {
        if (book[i + 1].num == 0)
            break;
        book[i] = book[i + 1];
        book[i].num -= 1;
    }
    strcpy(book[i].title, "");
    strcpy(book[i].author, "");
    book[i].pages = 0;
    book[i].price = 0;
    book[i].num = 0;
    show_info(5, book);
}