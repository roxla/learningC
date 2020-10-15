#include <math.h>
#include <stdio.h>

int main(void)
{
    int i, j, k, l, m, n;
    for (i = 4; i > 0; i--)
    {
        for (j = 0; j < i - 1; j++) // 输出空格
        {
            printf(" ");
        };
        for (k = 0; k < 7 - j * 2; k++) // 每行 "*" 的数量 = 总行数 - " " 的数量的两倍
        {
            printf("*");
        }
        printf("\n");
    };
    for (l = 0; l < 3; l++)
    {
        for (m = 0; m <= l; m++) // 输出空格
        {
            printf(" ");
        };
        for (n = 0; n < 7 - m * 2; n++)
        {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}

// int main(void)
// {
//     int i = 7, j, k, l;
//     int mid = (i + 1) / 2;
//     for (j = 1; j <= i; j++)
//     {
//         for (k = 0; k < abs(j - mid); k++)
//         {
//             printf(" ");
//         }
//         for (l = 0; l < i - 2 * abs(j - mid); l++) // 每行 "*" 的数量 = 总行数 - " " 的数量的两倍
//         {
//             printf("*");
//         }
//         printf("\n");
//     }

//     return 0;
// }
