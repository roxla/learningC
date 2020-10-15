#include <stdio.h>

#define STOP '#'

int main(void)
{
    char c;
    int space = 0, other = 0, enter = 0;
    printf("请输入字符: ");
    while ((c = getchar()) != STOP)
    {
        switch (c)
        {
        case '\n':
            enter += 1;
            break;
        case ' ':
            space += 1;
            break;
        default:
            other += 1;
            break;
        };
    };
    printf("空格数目: %d, 换行符数目: %d, 所有其他字符数目: %d\n", space, enter, other);

    return 0;
}
