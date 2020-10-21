#include <stdio.h>

#define NUM 10

int add_num(int num, int *arr);
int minus_num(int num, int *arr);
void get_rank(int *arr);
void show_num(int *arr);

int main(int argc, char const *argv[])
{
    int arr[NUM] = {0}, num;
    while (1)
    {
        printf("输入正数写入该正数，输入负数删除该负数，输入0退出程序\n");
        printf("请输入数字: ");
        scanf("%d", &num);
        while (getchar() != '\n')
            ;

        if (num > 0)
        {
            add_num(num, arr);
            show_num(arr);
        }
        else if (num < 0)
        {
            minus_num(-num, arr);
            show_num(arr);
        }
        else
            break;
    }

    return 0;
}

int add_num(int num, int *arr)
{
    int i;
    for (i = 0; i < NUM; i++)
    {
        if (arr[i] == 0)
            break; // 记录内容下标
    }
    if (i == NUM)
    {
        printf("\nFull!\n\n");
        return 0;
    }
    arr[i] = num;
    get_rank(arr);

    return 0;
}

int minus_num(int num, int *arr)
{
    int i;
    if (arr[0] == 0)
    {
        printf("\nNull!\n\n");
        return 0;
    }
    for (i = 0; i < NUM; i++)
    {
        if (arr[i] == num)
            break; // 记录内容下标
    }
    if (i == NUM)
    {
        printf("\nNot found!\n\n");
        return 0;
    }
    for (; i < NUM; i++)
    {
        arr[i] = arr[i + 1];
    }
    arr[NUM - 1] = 0;

    return 0;
}

void show_num(int *arr)
{
    printf("\n");
    int i;
    for (i = 0; i < NUM; i++)
    {
        if (arr[i] == 0)
            continue;
        printf("%d: %d  ", i + 1, arr[i]);
    }
    printf("\n\n");
}

void get_rank(int *arr)
{
    int i, j, tmp = 0;
    for (i = 0; i < NUM - 1; i++) //比较 NUM-1 次
    {
        for (j = 0; j < NUM - i - 1; j++) // 最后一次比较 a[NUM-i-1] 与 a[NUM-i-2]
        {
            if ((arr[j] > arr[j + 1]) && arr[j + 1] != 0) //如果 a[j] 比 a[j+1] 大且 a[j+1]!=0 则交换内容
            {
                tmp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}
