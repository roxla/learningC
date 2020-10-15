#include <stdio.h>

int main(void)
{
    int a, b, c, d;
    printf("请输入三角形的第一条边: ");
    scanf("%d", &a);
    while (getchar() != '\n')
        ;
    printf("请输入三角形的第二条边: ");
    scanf("%d", &b);
    while (getchar() != '\n')
        ;
    printf("请输入三角形的第三条边: ");
    scanf("%d", &c);
    while (getchar() != '\n')
        ;
    d = a;
    if (a > b) //a > b
    {
        if (a > c) //a > c
        {
            if (b < c) //a > c > b
            {
                d = b;
                b = c;
                c = d;
            }
        }
        else //c >= a > b
        {
            a = c;
            c = b;
            b = d;
        };
    }
    else //b >= a
    {
        if (b > c) // b > c
        {
            if (a > c) //b > a > c
            {
                a = b;
                b = d;
            }
            else //b > c >=a
            {
                a = b;
                b = c;
                c = d;
            }
        }
        else //c >= b >= a
        {
            a = c;
            c = d;
        };
    };
    if ((b + c) > a)
    {
        if ((b * b + c * c) == a * a)
        {
            printf("该三角形为直角三角形\n");
        }
        else if ((b * b + c * c) > a * a)
        {
            printf("该三角形为锐角三角形\n");
        }
        else
        {
            printf("该三角形为钝角三角形\n");
        };
    }
    else
    {
        printf("这三条边不能组成三角形\n");
    };

    getchar();
    return 0;
}