#include <stdio.h>

int main(void)
{
    int arr[2][3] = {{1, 3, 5}, {7, 9, 11}}, a;
    a = *arr[1];
        printf("%p", a);

    getchar();
    return 0;
}
