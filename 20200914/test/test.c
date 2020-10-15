#include<stdio.h>

int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    int* p1 = (int*)(&a + 1);
    int* p2 = (int*)((int)a + 1);
    int* p3 = (int*)(a + 1);
    printf("%d, %d, %d\n", p1[-1], p2[0], p3[1]);
    getchar();
    return 0;
}