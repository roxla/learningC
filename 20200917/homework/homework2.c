#include <stdio.h>

int main(void)
{
    char ABC;
    int i, j, k, l, num;
    printf("请输入一个大写字母: ");
    scanf("%c", &ABC);
    while (getchar() != '\n')
        ;
    num = ABC - 'A' + 1;
    for (i = num ; i > 0; i--) //总行数
    {
        for (j = 0; j < i; j++) //输出空格
        {
            printf(" ");
        };
        for (k = 0; k <= num - j; k++) //输出 A 到 ABC 之间的字母
        {
            printf("%c", 'A' + k);
        };
        for (l = k; l > 1; l--) //倒叙输出 A 到 ABC-1 之间的字母
        {
            /* 
                l=k，上一次循环结束时k自加了一次，所以 A + l 输出的值要比上一循环中 A + k 的值要大 1
                需要倒叙输出 A 到 ABC-1 之间的字母，那么就需要 A + l =  A + k - 1
                实际为  A + l =  A + k + 1，满足要求需要写成 A + l -2 
            */
            printf("%c", 'A' + l - 2);
        };
        printf("\n");
    };

    return 0;
}
