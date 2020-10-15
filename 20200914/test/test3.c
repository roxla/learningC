#include<stdio.h>

int main(void)
{
    char a;
    scanf("%c", &a);
    while (getchar()!='\n');
    printf("DEC: %d\n", a);
    printf("OCT: 0%o\n", a);
    printf("HEX: 0x%x\n", a);
    getchar();
    return 0;
}