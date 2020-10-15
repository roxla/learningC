#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = 100;
    int *p = NULL;
    p = &a;
    // if (p == NULL)
        
    printf("%d", *p);

    return 0;
}
