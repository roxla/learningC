#include <stdio.h>
#include <math.h>

void transform(double *arr, double *target, int n, double (*p)(double));

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

void transform(double *arr, double *target, int n, double (*p)(double))
{
    int i;
    for (i = 0; i < n; i++)
    {
        target[i] = p(arr[i]);
    };
}
