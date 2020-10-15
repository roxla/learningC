#include<stdio.h>

int main(void)
{
    char c1 = '\r';
    int c2 = 13, c3 = 015, c4 = 0xD;
    printf("转义序列输出%c\n", c1);
    printf("DEC输出%c\n", c2);    
    printf("OCT输出%c\n", c3);
    printf("HEX输出%c\n", c4);
    return 0;
}