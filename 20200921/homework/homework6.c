#include <stdio.h>

int Fibonacci(int n)
{
    if (n == 1 || n == 2)
    {
        return 1;
    }
    else
    {
        return Fibonacci(n - 1) + Fibonacci(n - 2);
    }
}
int main(void)
{
    int n = 0;
    int ret = 0;
    scanf("%d", &n);
    ret = Fibonacci(n);
    printf("ret=%d", ret);

    getchar();
    return 0;
}
