#include<stdio.h>

int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    int *p1 = (int*)(&a + 1); //数组a的地址的后一位的地址，未定义类型，所以需要使用(int*)
    printf("&a: %p\n", &a[4]);
    printf("&p1: %p\n", &p1);
    printf("&p1[-1]: %p\n", &p1[-1]);
    printf("*p1: %d\n", p1[-1]);
    getchar();
    return 0;
}