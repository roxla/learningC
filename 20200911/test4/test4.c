#include <stdio.h>

int main(void)
{
    int arr[] = {1,3,3,4,5};
    int *p = arr;
    // printf("%d\n", *p);
    int i = 0, length;
    length = sizeof(arr) / sizeof(arr[0]);
    for ( i = 0; i < length; i++)
    {
        printf("%d\n", *(p + i));
    };
    getchar();
    return 0;
}