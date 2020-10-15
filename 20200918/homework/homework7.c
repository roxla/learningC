#include<stdio.h>

int main(void)
{
    int arr[15] = {0}, arr1[10] = {[0 ... 4] = 8, [5 ... 7] = 2, [9] = 5}, arr2[10] = {[0 ... 4] = 8, [5 ... 7] = 2, [9] = 5};
    int length = sizeof(arr1) / sizeof(int), i;
    for (i = 0; i < length; i++)
    {
        arr[i] = arr1[i] + arr2[i];
    };

    return 0;
}
