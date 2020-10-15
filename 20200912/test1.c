#include <stdio.h>

int main(void){
    int arr[20], i, check;
    for (i = 0; i < 20; i++)
    {
        check = scanf("%d", &arr[i]);
        printf("check: %d\n", check);
    }
    getchar();
    return 0;
}