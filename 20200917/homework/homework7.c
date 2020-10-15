#include <stdio.h>

int main(void)
{
    int num;
    printf("请输入一个整数: ");
    scanf("%d", &num);
    while (getchar() != '\n')
        ;
    printf("0x%hhx\n", num);

    return 0;
}
