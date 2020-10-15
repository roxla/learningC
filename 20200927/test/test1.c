#include <stdio.h>

void recur(int a[], int k);

int main(void)
{
    int a[] = {1, 2, 3, 4, 5};
    int i;
    recur(a, 5);
    for (i = 0; i < 5; i++)
    {
        printf("%d", a[i]);
    }

    return 0;
}

void recur(int a[], int k) // 传入数组首元素和数组长度
{
    int tmp;
    if (k > 1)
    {
        /* 
            数组前一组首末交换后，需要交换的是后一组首末元素
            递归函数 recur 需要的参数为数组首元素和数组长度
            后一组首元素为 a[当前首元素下标+1] = a + 1
            (a + 1 后数组变为 a[] = {2, 3, 4, 5};)
            数组长度为 k - 前一组首末元素 = k - 2
            数组末位长度为 k - 1
        */
        recur(a + 1, k - 2); 
        tmp = a[0];
        a[0] = a[k - 1];
        a[k - 1] = tmp;
    }
}

// https://blog.csdn.net/vincent040/article/details/6096096
