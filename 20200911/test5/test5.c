#include <stdio.h>
#include<string.h>

int main(void)
{
    char arr[40], str[10];
    strcpy(arr, "This is runoob.com");
    printf("arr: %s\n", arr);
    strncpy(str, arr, 8);
    printf("str: %s\n", str);
    return 0;
}

// int main(void)
// {
//     char arr[40], str[40];
//     strcpy(arr,  "This is source");
//     strcpy(str, "This is destination");
//     strcat(str, arr);
//     printf("strcat: %s\n", str);
//     strncat(str, arr, 6);
//     printf("strncat: %s\n", str);
//     return 0;
// }