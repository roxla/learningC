#include <stdio.h>

int main(void)
{
    int i;
    char a = 'a';
    for (i = 0; i < 26; i++)
    {
        printf("%c ", a);
        a += 1;
    }
    printf("\n");

    return 0;
}
