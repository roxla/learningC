/*

1.构造一个结构体模板（图书信息），有下列成员：
序号	int
书名	char [20]
页数	int
价格	float

同时定义一个变量 stu1
可通过scanf循环输入不同的数字，执行不同的操作，如
1写入数据到该结构体变量中(可继续使用scanf输入)
2显示结构体变量所有信息

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 声明一个图书信息结构体
struct book_info
{
    int num;        //序号
    char title[20]; //书名
    int pages;      //页数
    float price;    //价格
};

// 显示信息函数
void show_info(int len, struct book_info *show);
// 输入图书信息函数
void input_info(int len, struct book_info *book);

int main(int argc, char const *argv[])
{
    int a;
    //局部变量，占用栈空间
    // 结构体数组，多个结构体
    struct book_info book[5] = {
        {1, "test", 60, 13.14},
        {2, "main", 100, 66.5},
    };
    // bzero(&book, 5*sizeof(struct book_info));

    while (1)
    {
        // 主界面提示
        printf("=======主界面=========\n");
        printf("1: 录入书籍信息\n"
               "2: 显示信息\n"
               "0: 退出\n");

        // 输入指令
        printf("Pls Input: ");
        scanf("%d", &a);
        while (getchar() != '\n')
            ;

        if (a == 1)
        {
            //录入书籍信息（自动录入最后一本）
            input_info(5, book);
        }
        else if (a == 2)
        {
            //显示信息
            show_info(5, book);
        }
        else if (a == 0)
            return 1;
    }

    return 0;
}

// 显示图书信息函数
// void show_info(int len, struct book_info show[])
// void show_info(int len, struct book_info show[len])
void show_info(int len, struct book_info *show)
{
    system("clear"); //调用 clear 指令，清屏
    printf("==============INFO=================\n");
    printf("序号\t 书名\t 页数\t 价格\n");

    int i;
    for (i = 0; i < len; i++)
        printf("%d\t %s\t %d\t %.3f\n",
               show[i].num,
               show[i].title,
               show[i].pages,
               show[i].price);

    // (show+i)->num,
    // (show+i)->title,
    // (show+i)->pages,
    // (show+i)->price);
    // 结构体指针->成员名
    // 结构体变量.成员名
}

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
