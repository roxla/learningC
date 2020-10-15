#include <stdio.h>

double dValue(double arr[], int length);

int main(void)
{
    double arr[10] = {0}, d_value;
    int i, length = sizeof(arr) / sizeof(double);
    for (i = 0; i < length; i++)
    {
        printf("请输入第%d个浮点数: ", i + 1);
        scanf("%lf", &arr[i]);
        while (getchar() != '\n')
            ;
    };
    d_value = dValue(arr, length);
    printf("======= DATA =======\n");
    printf("最大和最小值的差值为:　%.2lf\n", d_value);

    return 0;
}

double dValue(double arr[], int length)
{
    double max = arr[9], min = arr[0];
    int i;
    for (i = 0; i < length; i++)
    {
        if (max < arr[i])
            max = arr[i];
        if (min > arr[i])
            min = arr[i];
    };
    return max - min;
}
