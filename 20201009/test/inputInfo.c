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

// 输入图书信息函数
void input_info(int len, struct book_info *book)
{
    // 1.数组遍历，找出空位，记录下标i
    int i;
    for (i = 0; i < len; i++)
    {
        if (book[i].num == 0)
            break;
    }
    // 如果循环完毕（i==5）都没有找到，说明没有空位（满了）
    if (i == len)
    {
        printf("Full!!!\n");
        return;
    }

    // 2.将数据写入到该下标的位置。
    //书籍序号
    book[i].num = i + 1;

    printf("书名: ");
    scanf("%s", book[i].title);
    while (getchar() != '\n')
        ;

    printf("作者名: ");
    scanf("%s", book[i].author);
    while (getchar() != '\n')
        ;

    printf("页数: ");
    scanf("%d", &book[i].pages);
    while (getchar() != '\n')
        ;

    printf("价格: ");
    scanf("%f", &book[i].price);
    while (getchar() != '\n')
        ;

    printf("图书(%d)信息录入成功！\n", book[i].num);
    show_info(5, book);
}
