#include <stdio.h>

int main(void)
{
    int n, m, check;
    while (1)
    {
        printf("请输入一个整数:");
        check = scanf("%d", &n);
        while (getchar() != '\n');
        if (check == 1)
        {
            break;
        }
        else
        {
            printf("输入的不是数字\n");
        };
    };
    m = n;
    printf("小于%d的奇数有:\n", n);
    while (m)
    {
        m--;
        if (m % 2 != 0)
        {
            printf("%d ", m);
        }
    };
    getchar();
    return 0;
}