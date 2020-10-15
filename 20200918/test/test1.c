#include <stdio.h>

int main(void)
{
    int arr[10] = {[0 ... 4] = 666, [5 ... 7] = 999}, i;
    int length = sizeof(arr) / sizeof(int), *p = arr;
    for (i = 0; i < length; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("==========================\n");
    *(p + 9) += 1;
    for (i = 0; i < length; i++)
    {
        printf("%d\n", p[i]);
    }

    getchar();
    return 0;
}
