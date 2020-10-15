#include<stdio.h>

int main(void)
{
    int a = 4, b = 3, *p, *q, *w;
    p = &a;
    q = &b;
    w = p;
    q = NULL; //存储的地址被清除了，解引用后的值也被清除了，所以指针 q 是空的
    // printf("%d", *q); 此步报错

    getchar();
    return 0;
}
