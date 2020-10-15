#include <stdio.h>
#include <string.h>
#define DEFUSER "roxla"
#define DEFPWD "a123456"

int login(void)
{
    static int err = 0; //密码错误次数
    char user[20] = "", pwd[20] = "";
    int checkUser, checkPwd;
    if (err >= 3)
        return 3; //输错3次密码退出
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
        err = 0;
        return 1;
    }
    else if (checkUser != 0)
    {
        printf("用户名错误!\n");
        return 2;
    }
    else if (checkUser == 0 && checkPwd != 0)
    {
        printf("密码错误!(错误次数%d次)\n", err + 1);
        err += 1;
        return 2;
    }

    return 0;
}