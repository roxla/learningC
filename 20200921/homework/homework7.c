#include <stdio.h>

int clear_Space(char *a, int i);
char *check_Space(char *a);

int main(void)
{
    char str[30] = "hello world!";
    char *p = NULL;
    p = check_Space(str);
    printf("%s\n", p);

    getchar();
    return 0;
}

char *check_Space(char *a)
{
    char *pa = a;
    int i = 0, j = 0;
    while (1)
    {
        if (pa[j] == '\0')
            break;
        if (i < 30)
        {
            i = clear_Space(pa, i);
            i++;
        }
        pa[j] = pa[i - 1];
        j++;
    }
    return pa;
}

int clear_Space(char *a, int i)
{
    if (a[i] == ' ')
    {
        return clear_Space(a, i + 1);
    }
    return i;
}