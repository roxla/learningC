#include <stdio.h>

int main(void)
{
    int a, i;
    printf("请输入一个整数: ");
    scanf("%d", &a);
    while (getchar() != '\n')
        ;
    for (i = 0; i <= 10; i++)
    {
        printf("%d ", a++);
    }
    
    getchar();
    return 0;
}
