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
#define DEFUSER "roxla"
#define DEFPWD "a123456"

// 声明一个图书信息结构体
struct book_info
{
    int num;        //序号
    char title[20]; //书名
    int pages;      //页数
    float price;    //价格
};

// 登录
int login(void);
// 显示信息函数
void show_info(int len, struct book_info *show);
// 输入图书信息函数
void input_info(int len, struct book_info *book);
// 删除图书信息函数
void delete_info(int len, struct book_info *book);
// 查询图书信息函数
void serch_info(int len, struct book_info *show);
// 修改图书信息函数
void change_info(int len, struct book_info *book);

int main(int argc, char const *argv[])
{
    int a, log = 0;
    log = login();
    if (log >= 3)
    {
        exit(0);
    };
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
        printf("1: 显示信息\n"
               "2: 录入书籍信息\n"
               "3: 删除信息\n"
               "4: 查询信息\n"
               "5: 修改信息\n"
               "0: 退出\n");

        // 输入指令
        printf("Pls Input: ");
        scanf("%d", &a);
        while (getchar() != '\n')
            ;
        if (a == 1)
            // 显示信息
            show_info(5, book);
        else if (a == 2)
            // 录入书籍信息（自动录入最后一本）
            input_info(5, book);
        else if (a == 3)
            // 删除信息
            delete_info(5, book);
        else if (a == 4)
            // 查询信息
            serch_info(5, book);
        else if (a == 5)
            // 修改信息
            change_info(5, book);
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
    for (i = num - 1; i < len - num; i++)
    {
        strncpy(book[i].title, book[i + 1].title, sizeof(book[i].title));
        book[i].pages = book[i + 1].pages;
        book[i].price = book[i + 1].price;
    }
    strcpy(book[len - 1].title, "NULL");
    book[len - 1].pages = 0;
    book[len - 1].price = 0;
    book[len - 1].num = 0;
    show_info(5, book);
}

// 查询图书信息函数
void serch_info(int len, struct book_info *show)
{
    char arr[20];
    unsigned int i, num;
    float cost;
    printf("请输入要查询的信息: ");
    scanf("%s", arr);
    while (getchar() != '\n')
        ;
    /* 
        判断输入字符串是否为数字
        strspn(char str1, char str2)：返回 str1 中第一个不在字符串 str2 中出现的字符下标
        strlen(char str)：返回字符串的长度
    */
    if (strspn(arr, "0123456789") == strlen(arr))
        // https://blog.csdn.net/weixin_34364071/article/details/86054032
        num = atoi(arr);
    else if (strspn(arr, "0123456789.") == strlen(arr))
        cost = atof(arr);
    system("clear"); //调用 clear 指令，清屏
    printf("==============INFO=================\n");
    printf("序号\t 书名\t 页数\t 价格\n");
    for (i = 0; i < len; i++)
    {
        if (
            show[i].num == num ||
            show[i].pages == num ||
            show[i].price == cost ||
            strcmp(show[i].title, arr) == 0)
        {
            printf("%d\t %s\t %d\t %.3f\n",
                   show[i].num,
                   show[i].title,
                   show[i].pages,
                   show[i].price);
        }
    }
}

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

// 登录
int login(void)
{
    static int err = 0;
    char user[20] = "", pwd[20] = "";
    int checkUser, checkPwd;
    if (err < 3)
    {
        printf("请输入用户名: ");
        scanf("%s", user);
        while (getchar() != '\n')
            ;
        printf("请输入密码: ");
        scanf("%s", pwd);
        while (getchar() != '\n')
            ;
        checkUser = strcmp(user, DEFUSER);
        checkPwd = strcmp(pwd, DEFPWD);
        if (checkUser == 0 && checkPwd == 0)
        {
            printf("欢迎用户 %s 使用图书管理系统!\n", user);
        }
        else if (checkUser != 0)
        {
            printf("用户名错误!\n");
            login();
        }
        else if (checkUser == 0 && checkPwd != 0)
        {
            printf("密码错误!\n");
            err++;
            login();
        }
    }

    return err;
}