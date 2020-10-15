#include <stdio.h>

int sum_n(int n, int *sum);

int main(void)
{
    int n, sum = 0;
    int *p = &sum;
    scanf("%d", &n);
    while (getchar() != '\n')
        ;
    sum_n(n, p);
    printf("%d\n", *p);

    return 0;
}

int sum_n(int n, int *sum)
{
    if (n == 0)
    {
        return 0;
    }
    *sum += n;
    return sum_n(n - 1, sum);
}
