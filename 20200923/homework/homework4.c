#include <stdio.h>

int Count();

int main(int argc, char const *argv[])
{
    int i, j;
    printf("请输入调用次数: ");
    scanf("%d", &j);
    while (getchar() != '\n')
        ;
    for (i =1; i <= j; i++)
        printf("第%d次调用，返回值=%d\n", i, Count());

    return 0;
}

int Count()
{
    static int count = 0; // 静态数据段    生命周期与进程相当    static修饰的变量（局部、全局）
    return ++count;
}
