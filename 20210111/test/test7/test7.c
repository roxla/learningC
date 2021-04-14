#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(void)
{
    char str[] = "raysharp.cn"; //raysharp.cn = 11, char str[12], 结尾有一个 /0
    printf("str = %ld\n",sizeof(str));
    void *p = malloc(100);
    printf("p = %ld\n",sizeof(p));
    free(p);
}
