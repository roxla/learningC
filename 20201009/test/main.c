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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SUM 5
#define LOGSUM 3 //登陆次数

// 声明一个图书信息结构体
struct book_info
{
    int num;         //序号
    char title[20];  //书名
    char author[20]; //作者名
    int pages;       //页数
    float price;     //价格
};

/******************功能函数声明***********************/
// 初始化3本图书信息
extern void init_book(struct book_info *book);
// 登录
extern int login(void);
// 显示信息函数
extern void show_info(int len, struct book_info *show);
// 输入图书信息函数
extern void input_info(int len, struct book_info *book);
// 删除图书信息函数
extern void delete_info(int len, struct book_info *book);
// 查询图书信息函数
extern void serch_info(int len, struct book_info *show);
// 修改图书信息函数
extern void change_info(int len, struct book_info *book);

int main(int argc, char const *argv[])
{
    int cmd;
    while (1)
    {
        if (login() == 1)
            break;
        else if (login() == 3)
            exit(0);
    }

    //局部变量，占用栈空间
    // 结构体数组，多个结构体
    // struct book_info book[SUM] = {
    //     {1, "test", 60, 13.14},
    //     {2, "main", 100, 66.5},
    // };
    // bzero(&book, 5*sizeof(struct book_info));
    struct book_info *book = calloc(SUM, sizeof(struct book_info));
    if (book == NULL)
    {
        // 如果申请失败，结束程序
        perror("book_info calloc failed");
        return 1;
    }
    bzero(book, SUM * sizeof(struct book_info)); //清空堆空间
    // 初始化写入3本图书的信息
    init_book(book);

    while (1)
    {
        // 主界面提示
        printf("=======主界面=========\n");
        printf("1: 显示信息\n"
               "2: 录入书籍信息\n"
               "3: 删除信息\n"
               "4: 查询信息\n"
               "5: 修改信息\n"
               "0: 退出\n");

        // 输入指令
        printf("Pls Input: ");
        scanf("%d", &cmd);
        while (getchar() != '\n')
            ;
        if (cmd == 1)
            // 显示信息
            show_info(SUM, book);
        else if (cmd == 2)
            // 录入书籍信息（自动录入最后一本）
            input_info(SUM, book);
        else if (cmd == 3)
            // 删除信息
            delete_info(SUM, book);
        else if (cmd == 4)
            // 查询信息
            serch_info(SUM, book);
        else if (cmd == 5)
            // 修改信息
            change_info(SUM, book);
        else if (cmd == 0)
            return 1;
    }

    return 0;
}