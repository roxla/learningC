#include<stdio.h>

int main()
{
    char a, b;
    scanf("%c", &a);
    while (getchar()!='\n')
        ;
    b = a - 32;
    printf("%c", b);
    getchar();
    return 0;
}