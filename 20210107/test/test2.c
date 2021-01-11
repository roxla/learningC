//将整数转化成字符串数，不用函数itoa
#include <stdio.h>

int main(void)
{
    int num = 12345, j = 0, i = 0;
    char temp[7], str[7];

    while (num)
    {
        temp[i] = num % 10 + '0'; //整数加‘0’，隐性转化成char类型的数
        i++;
        num = num / 10;
    }
    temp[i] = 0;
    printf(" temp = %s\n", temp); //输出char类型的逆序数组
    i = i - 1;
    printf(" temp = %d\n", i); //输出数组最高位的下标
    while (i >= 0)             //将char类型的数组正序存入str数组中
    {
        str[j] = temp[i];
        j++;
        i--;
    }
    str[j] = 0;
    printf(" string = %s\n", str); //输出最终数组结果
    return 0;
}