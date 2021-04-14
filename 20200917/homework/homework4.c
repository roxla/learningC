#include <stdio.h>

/*
素数：又称质数。所谓素数是指除了 1 和它本身以外，不能被任何整数整除的数

思路：判断一个整数 num 是否是素数，只需把 num 被 2 ~ num-1 之间的每一个整数去除，如果都不能被整除，那么 num 就是一个素数

算法：定义一个计数器 count，循环判断 num 能否被 2 ~ num-1 之间的某一个整数整除，能则 count+1，
      循环结束后判断 count 是否为 0，为 0 则 num 为素数
*/

void comp(int num)
{
    int i, count = 0;
    for (i = 2; i < num; i++)
    {
        if (num % i == 0)
        {
            count++;
        };
    };
    if (count == 0)
    {
        printf("%d\n", num);
    };
}

int main(void)
{
    int num;
    printf("请输入一个大于1的正整数: ");
    scanf("%d", &num);
    while (getchar() != '\n')
        ;
    while (1)
    {
        if (num > 1)
        {
            comp(num--);
        }
        else
        {
            printf("end\n");
            break;
        };
    };

    return 0;
}
