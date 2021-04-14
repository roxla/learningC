#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

void main(void)
{
    srand(time(NULL));
    int num, len, i;
    int arr[10] = {1,2,3,4,5,6,7,8,9,0};
    len = ARRAY_COUNT(arr);
    while(len)
    {
        num = rand()%len;
        printf("arr = %d\n",arr[num]);
        // printf("num = %d\n",num);
        // printf("len = %d\n",len);
        for(i = num; i < len; i++)
            arr[i] = arr[i+1];
        arr[i - 1] = 0;
        len--;
    }
}