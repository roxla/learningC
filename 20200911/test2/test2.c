#include <stdio.h>

int main(void)
{
    char arr[20] = "hello,123";
    arr[0] = 'H'; //?
    arr[6] = 'J';
    arr[7] = 'a';
    arr[8] = 'c';
    arr[9] = 'k';
    printf("%s", arr);
    getchar();
    return 0;
}