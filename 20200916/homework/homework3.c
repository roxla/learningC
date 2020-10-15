#include <stdio.h>

float test1(float a)
{
    return a * a * a;
}

int main(void)
{
    float a, b;
    printf("请输入一个浮点数: ");
    scanf("%f", &a);
    while (getchar() != '\n')
        ;
    b = test1(a);
    printf("%f\n", b);

    getchar();
    return 0;
}