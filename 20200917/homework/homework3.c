#include <stdio.h>

int main(void)
{
    int space = 0, other = 0, enter = 0, loop = 1;
    char Input;
    while (loop)
    {
        printf("请输入一个字符: ");
        scanf("%c", &Input);
        while (getchar() != '\n')
            ;
        switch (Input)
        {
        case '\n':
            enter += 1;
            break;
        case ' ':
            space += 1;
            break;
        case '#':
            loop = 0;
            break;
        default:
            other += 1;
            break;
        }
    };
    printf("空格数目: %d, 换行符数目: %d, 所有其他字符数目: %d\n", space, enter, other);

    return 0;
}
