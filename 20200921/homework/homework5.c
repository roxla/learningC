#include <stdio.h>

char *str_Cat(char *a, char *b)
{
    char *p1 = a, *p2 = b;
    int i = 0, j = 0;
    while (1)
    {
        if (*(p1 + i) == '\0')
            break;
        i++;
    }
    while (1)
    {
        if (*(p2 + j) == '\0')
            break;
        p1[i + j] = p2[j];
        j++;
    }
    return p1;
}

int main(void)
{
    char a[21] = "hello", b[10] = " world!";
    char *p = NULL;
    p = str_Cat(a, b);
    printf("%s", p);

    getchar();
    return 0;
}
