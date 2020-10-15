#include<stdio.h>

int main(void)
{
    float C, F;
    printf("请输入一个华氏度: ");
    scanf("%f", &F);
    while (getchar() != '\n')
        ;
    C = 5 * (F - 32) / 9;
    printf("%.2f℉ 对应的摄氏度为: %.2f℃ \n", F, C);

    return 0;
}
