#include <stdio.h>

int main(void)
{
    int arr[2][3] = {{1, 3, 5}, {7, 9, 11}};
    int *p = arr[0];
    p[2] = 10;
    printf("%d", p[2]);

    getchar();
    return 0;
}
