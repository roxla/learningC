#include <stdio.h>
int main(void)
{
    int ref[] = {8, 4, 0, 2};
    int *p = ref;
    printf("%p\n", ref);
    printf("%p\n", ++p);

    getchar();
    return 0;
}