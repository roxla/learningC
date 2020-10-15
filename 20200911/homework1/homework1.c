#include<stdio.h>
#include<string.h>

int main(void)
{
    char user[10] = {0}, pwd[20] = {0}, defaultUser[] = "admin", defaultPwd[] = "123456";
    while (1)
    {
        printf("请输入用户名: ");
        scanf("%s", user);
        while (getchar() != '\n');
        printf("请输入密码: ");
        scanf("%s", pwd);
        while (getchar() != '\n');
        if(strcmp(user, defaultUser) == 0 && strcmp(pwd, defaultPwd) == 0)
        {
            printf("Welcome,%s\n", user);
            break;
        }
        else if(strcmp(user, defaultUser) != 0)
        {
            printf("无此用户\n");
        }
        else if(strcmp(user, defaultUser) == 0 && strcmp(pwd, defaultPwd) != 0)
        {
            printf("密码错误\n");
        }
    }
    getchar();
    return 0;
}
