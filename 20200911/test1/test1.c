#include <stdio.h>

int main(void)
{
    int arr[15], length, check, i = 0, loop, j, num;
    length = sizeof(arr) / sizeof(arr[0]);
    loop = length;
    while (loop)
    {
        printf("请输入数字%d/15:", i + 1);
        check = scanf("%d", &num);
        while (getchar() != '\n');
        if (check == 1)
        {
            arr[i] = num;
            i++;
            loop--;
        }
        else
        {
           printf("error\n");
        }
    }
    printf("========================\n");
    for (j = 0; j < length; j++)
    {
        printf("%d ", arr[j]);
    }
    getchar();
    return 0;
}