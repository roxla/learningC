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

// 初始化3本图书信息
void init_book(struct book_info *book)
{
    book[0].num = 1;
    strncpy(book[0].title, "abc", sizeof(book[0].title));
    strncpy(book[0].author, "Tom", sizeof(book[0].author));
    book[0].pages = 100;
    book[0].price = 54.5;

    book[1].num = 2;
    strncpy(book[1].title, "789", sizeof(book[1].title));
    strncpy(book[1].author, "Tom", sizeof(book[1].author));
    book[1].pages = 560;
    book[1].price = 88.5;

    book[2].num = 3;
    strncpy(book[2].title, "qwer", sizeof(book[2].title));
    strncpy(book[2].author, "Jerry", sizeof(book[2].author));
    book[2].pages = 700;
    book[2].price = 108.5;
}