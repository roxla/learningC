#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = -1;
    if (++a)
        printf("1\n");
    else if (a++)
        printf("2\n");
    else if (a++)
        printf("3\n");
    else if (a++)
        printf("4\n");
    else
        printf("5\n");
    printf("a: %d\n", a);

    return 0;
}
