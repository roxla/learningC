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
    printf("序号\t 书名\t 作者名\t 页数\t 价格\n");
    for (i = 0; i < len; i++)
    {
        if (
            show[i].num == num ||
            show[i].pages == num ||
            show[i].price == cost ||
            strcmp(show[i].title, arr) == 0 ||
            strcmp(show[i].author, arr) == 0)
        {
            printf("%d\t %s\t %s\t %d\t %.3f\n",
                   show[i].num,
                   show[i].title,
                   show[i].author,
                   show[i].pages,
                   show[i].price);
        }
    }
    if (i == len)
    {
        printf("未找到该书籍!");
    }
}
