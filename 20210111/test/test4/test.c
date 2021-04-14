#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3
#define Y(n)((N+1)*n)

typedef struct
{
    short a;
    char b;
    float c;
}cs;

void main(void)
{
    printf("%d\n",2*(N+Y(5+1)));
}
