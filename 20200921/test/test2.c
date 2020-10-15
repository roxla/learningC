#include <stdio.h>
#include <string.h>

void strip(char *arr, int length);

int main(void)
{
    char arr[30] = "apple  hello world  !";
    int length = strlen(arr);
    strip(arr, length);

    return 0;
}

void strip(char *arr, int length)
{
    int i, j = 0;
    char str[30] = "";
    char *p = str;
    for (i = 0; i < length; i++)
    {
        if (arr[i] != ' ')
        {
            p[j] = arr[i];
            j++;
        }
    }
    printf("%s", p);
}