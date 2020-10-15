#include <stdio.h>

int main(void)
{
    int arr[2][3] = {0}, i, j;
    float sum, avg;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
        {
            scanf("%d", &arr[i][j]);
            while (getchar() != '\n')
                ;
        };
    };
    printf("===== DATA =====\n");
    for (i = 0; i < 2; i++)
    {
        sum = 0;
        for (j = 1; j < 3; j++)
        {
            sum += arr[i][j];
        };
        printf("%.0f\n", sum);
    };
    printf("===== DATA =====\n");
    for (j = 1; j < 3; j++)
    {
        for (i = 0; i < 2; i++)
        {
            avg += arr[i][j];
        };
        printf("%.0f\n", avg / 2);
    };

    return 0;
}