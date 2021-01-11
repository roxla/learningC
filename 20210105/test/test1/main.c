#include <stdio.h>
#include "sqlite3.h"

void addData(sqlite3 *mydb);
void searchData(sqlite3 *mydb);
int fun(void *arg, int col, char **str1, char **str2);
void changeData(sqlite3 *mydb);
void delData(sqlite3 *mydb);

int main(int argc, char const *argv[])
{
    sqlite3 *mydb;
    int ret;
    ret = sqlite3_open("./book.db", &mydb);
    if (ret != SQLITE_OK)
    {
        printf("open error!");
        return -1;
    }
    // 新建表格
    char *err;
    ret = sqlite3_exec(mydb, "create table if not exists book (name text,author text,price float);", NULL, NULL, &err);
    if (ret != SQLITE_OK)
    {
        printf("%s\n", err);
        return -1;
    }
    //
    int loop = 1;
    int num;
    while (loop)
    {
        printf(
            "1.添加数据\n"
            "2.修改数据\n"
            "3.删除数据\n"
            "4.查询数据\n"
            "0.退出\n");
        scanf("%d", &num);
        while (getchar() != '\n')
            ;

        switch (num)
        {
        case 1:
            addData(mydb);
            break;
        case 2:
            changeData(mydb);
            break;
        case 3:
            delData(mydb);
            break;
        case 4:
            searchData(mydb);
            break;
        case 0:
            loop = 0;
            break;
        }
    }

    return 0;
}

void addData(sqlite3 *mydb)
{
    char name[10];
    printf("pls input book name:");
    scanf("%s", name);
    while (getchar() != '\n')
        ;
    char author[10];
    printf("pls input book author:");
    scanf("%s", author);
    while (getchar() != '\n')
        ;
    float price;
    printf("pls input book price:");
    scanf("%f", &price);
    while (getchar() != '\n')
        ;

    char sql[100];
    snprintf(sql, sizeof(sql), "insert into book values (\"%s\",\"%s\",%.02f);", name, author, price);

    int ret;
    char *err;
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &err);
    if (ret != SQLITE_OK)
    {
        printf("%s\n", err);
        return;
    }
}

void searchData(sqlite3 *mydb)
{
    int ret;
    char *err;
    //当你执行查询语句的时候，必须传递第三个参数，否则无法看到查询结果
    ret = sqlite3_exec(mydb, "select * from book;", fun, NULL, &err);
    if (ret != SQLITE_OK)
    {
        printf("查询数据失败，失败原因: %s\n", err);
        return;
    }
}

//自定义一个函数(执行查询语句必须用到)
int fun(void *arg, int col, char **str1, char **str2)
{
    int i;
    // printf("你查询的booktable总共有:%d列\n",col);
    for (i = 0; i < col; i++)
    {
        // printf("str1中存放的是:%s\n",*(str1+i));
        // printf("str2中存放的是:%s\n",*(str2+i));
        printf("%s:%s\n", *(str2 + i), *(str1 + i));
    }
    // static int count=0; //静态局部变量只能初始化一次，用来统计fun函数调用了几次
    // count++;
    // printf("目前fun被调用了%d次!\n",count);
    return 0;
}

void changeData(sqlite3 *mydb)
{
    char name[10];
    printf("pls input book name:");
    scanf("%s", name);
    while (getchar() != '\n')
        ;
    float price;
    printf("pls input book price:");
    scanf("%f", &price);
    while (getchar() != '\n')
        ;

    char sql[100];
    snprintf(sql, sizeof(sql), "update book set price=%.02f where name==\"%s\";", price, name);

    int ret;
    char *err;
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &err);
    if (ret != SQLITE_OK)
    {
        printf("%s\n", err);
        return;
    }
}

void delData(sqlite3 *mydb)
{
    char name[10];
    printf("pls input book name:");
    scanf("%s", name);
    while (getchar() != '\n')
        ;

    char sql[100];
    snprintf(sql, sizeof(sql), "delete from book where name==\"%s\";", name);

    int ret;
    char *err;
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &err);
    if (ret != SQLITE_OK)
    {
        printf("%s\n", err);
        return;
    }
}