#include<stdio.h>

int main(void)
{
    int a;
    printf("please enter a ASCII:");
    scanf("%d", &a);
    while (getchar()!='\n');
    printf("the input ASCII ma is '%c'\n",a);
    return 0;
}