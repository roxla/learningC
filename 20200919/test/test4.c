#include <stdio.h>

int main(void)
{
    int a[2][3] = {{1, 3, 5}, {7, 9, 11}};
    int(*p1)[3] = a;
    int sum = 0, i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
            sum += (*(p1 + i))[j];
    printf("sum: %d\n", sum);
    printf("%d\n", *(*(p1+1)+2));

    getchar();
    return 0;
}

/* 
    p1 = a
    *p1 = a[0]
    *(*p1) = a[0][0] => **p1 = a[0][0]      1
    (*p1[0])[2] = a[0][2] => (*p1)[2]       5
    (*p1[1])[2] = a[1][2] => *(*(p1+1)+2)   11
*/