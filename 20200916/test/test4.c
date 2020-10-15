#include<stdio.h>

int main(void)
{
    int a = 12;
    int b = ++a;
    b++;

    printf("1: %d\n", a++);
    printf("2: %d\n", ++b);
    printf("3: %d\n", a);
    printf("4: %d\n", b);

    getchar();
    return 0;
}