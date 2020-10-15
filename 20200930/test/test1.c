#include <stdio.h>

int main(int argc, char const *argv[])
{
    unsigned int a = 0x1234;
    char *p = (char *)&a;
    printf("0x%hx\n", *p);

    return 0;
}
