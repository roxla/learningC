#include<stdio.h>

int main(void)
{
    double water, quart, m = 3.0e-23;
    printf("请输入水的夸脱数: ");
    scanf("%lf", &quart);
    while (getchar()!='\n');
    water = quart * 950 / m;
    printf("%f夸脱的水中包含%e个水分子。\n", quart, water);
    return 0;
}