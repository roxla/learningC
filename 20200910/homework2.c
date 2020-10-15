#include <stdio.h>

int main(void)
{
    int n, check, i;
    while (i < 100)
    {
        printf("请输入一个数字:");
        check = scanf("%d", &n);
        while (getchar() != '\n');
        if (check == 1)
        {
            switch (n)
            {
            case 1:
                printf("roxla\n");
                break;
            case 2:
                printf("GZ2067\n");
                break;
            case 3:
                printf("11111111111\n");
                break;
            default:
                i = 100;
                break;
            }
        }
        else
        {
            printf("输入的不是数字\n");
        }
    }
}