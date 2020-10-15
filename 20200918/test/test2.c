#include <stdio.h>

int main(void)
{
    int arr[5] = {0}, i, max = 0;
    int length = sizeof(arr) / sizeof(int);
    for (i = 0; i < length; i++)
    {
        printf("请输入第%d只猪的体重: ", i + 1);
        scanf("%d", &arr[i]);
        while (getchar() != '\n')
            ;
    };
    for (i = 0; i < length; i++)
    {
        if (max < arr[i])
            max = arr[i];
    };
    printf("最重的猪的体重为%d公斤\n", max);

    return 0;
}