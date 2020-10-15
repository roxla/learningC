#include <stdio.h>

float my_power(float a, int N)
{
    if (N > 0)
    {
        return my_power(a, N - 1) * a;
    }
    else if (N < 0)
    {
        return my_power(a, N + 1) * (1 / a);
    }
    else if (N == 0)
    {
        return 1;
    }
}

int main(int argc, char const *argv[])
{
    float a, sum;
    int N;
    printf("请输入数字a以及整数次幂N\n");
    scanf("%f %d", &a, &N);
    while (getchar() != '\n')
        ;
    sum = my_power(a, N);
    printf("%.6f\n", sum);
    return 0;
}