#include <stdio.h>

int check_Max(int a, int b, int c)
{
    if(a>b){
        return a > c ? a : c;
    }else
    {
        return b > c ? b : c;
    }
    
}

int main(void)
{
    int a = 4, b = 6, c = 5, d;
    d = check_Max(a, b, c);
    printf("%d", d);

    getchar();
    return 0;
}
