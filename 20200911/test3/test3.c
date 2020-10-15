#include <stdio.h>

int main(void)
{
    char str[10] = {0}, arr[] = "hello,123";
    int i = 0;
    scanf("%s", str);
    while (getchar() != '\n');
    while (1)
    {
        if(str[i] == '\0')
        {
            break;
        }
        arr[6 + i] = str[i];
        i++;
    }   
    printf("%s", arr);
    getchar();
    return 0;
}