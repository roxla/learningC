#include <stdio.h>

#define SEQ_LEN 10

// 显示顺序表(Sequence table)
// n表示数据下标
int seq_show(int *p, int n);
// 插入到数据到顺序表中
void seq_add(int new_data, int *p, int *n);
// 从顺序表中删除数据
int seq_del(int del_data, int *p, int *n);

int main(void)
{
    // 1.初始化顺序表
    // A.定义一个数组，用于存储数据（数据域）
    int arr[10] = {0};
    // B.定义一个整型变量，用于存储最后一个数据的下标（下标域）
    int i = -1; // -1表示当前无数据

    // 2.操作顺序表
    int num; //num用来记录用户输入的数字
    while (1)
    {
        fprintf(stderr, "Pls Input: ");
        scanf("%d", &num);
        while (getchar() != '\n')
            ;

        if (num > 0)
        {
            // 2.1 增加该数据到顺序表中
            seq_add_ans(num, arr, &i);
        }
        else if (num < 0)
        {
            // 2.2 从顺序表中删除该数据的绝对值
            if(seq_del(-num, arr, &i)) // 删除数组内的 num，如果有相同的数，则一起删除
                seq_del(-num, arr, &i);
        }
        else
        {
            // 2.3 退出程序
            printf("ByeBye~\n");
            return 0;
        }
        // 2.4 显示顺序表中的数据
        seq_show(arr, i);
    }

    return 0;
}

// 数据的显示
int seq_show(int *p, int n)
{
    printf("======DATA=====\n");
    int i;
    for (i = 0; i < n + 1; i++)
    {
        printf("%d ", p[i]);
    }
    printf("\n");

    return 0;
}

// 数据顺序添加
void seq_add_ans(int new_data, int *p, int *n)
{
	// 1.判断顺序表是否已满
	if (*n == SEQ_LEN - 1)
	{
		printf("Full!\n");
		return;
	}
	// 2.遍历顺序表，找出比新数据更大的数，记录下标
	int i, j;
	for (i = 0; i < *n + 1; i++)
	{
		if (new_data <= p[i])
			break;
	}
	// 3.下标以后的数据全部往后挪动（从最后一个数据开始）
	for (j = *n; j >= i; j--)
		p[j + 1] = p[j];
	// 4.将新数据写入到指定下标
	p[i] = new_data;
	// 5.下标域++
	(*n)++;
}

// 数据的添加
// void seq_add(int new_data, int *p, int *n)
// {
//     // 1.判断顺序表是否已满？
//     if (*n == SEQ_LEN - 1)
//     {
//         printf("Full!\n");
//         return;
//     }

//     // 2.将数据写入到顺序表中
//     // （写入位置：下标域+1）
//     p[*n + 1] = new_data;

//     // 3.下标域++
//     (*n)++;
// }

// 从顺序表中删除数据
int seq_del(int del_data, int *p, int *n)
{
    // 根据伪代码，编程实现功能。
    // 1.判断顺序表是否为空？
    // （判断下标域不等于-1）
    if (*n == -1)
    {
        printf("Null!\n");
        return 0;
    }
    // 2.遍历顺序表，对比欲删除的数据，记录下标
    int i;
    for (i = 0; i < *n + 1; i++)
    {
        if (p[i] == del_data)
            break;
    }
    // 3.如果没有找到欲删除的数据
    if (i == *n + 1)
        return 0;
    // 4.根据这个下标，将后面的数据往前覆盖，
    // 直到下标域。
    for (; i < *n; i++)
        p[i] = p[i + 1];
    // 5.下标域--
    (*n)--;

    return 1;
}

/*
		show_arr(10, arr);
		printf("Pls input a number:");
		scanf("%d", &num);while(getchar() != '\n');
		if (num == 0)
		{
			printf("bye!\n");
			return 0;
		}

		//1.用户输入的数字大于0，往数组里增加
		if (num > 0)
		{
			if(i > 9)
			{
				printf("full!\n");	//提示数组已经满了
				continue;
			}
			arr[i] = num;
			i++;
		}

		//2.用户输入的数字小于0，在数组里减少；
		int k = 0;	//用来判断是否找到要删除的数值；
		while(num < 0)	//使用while循环的原因是，如果数组里有相同的数，则需要多次循环删除；
		{
			if(i < 1)
			{
				printf("empty!\n");
				break;
			}
			int j;		//j用来记录所要删除数值的位置；
			
			for (j = 0; j <= i; j++)
			{
				if(arr[j] == -num)
				{
					k++;
					break;
				}
			}
			//如果第一次循环都没有找到要删除的数值，直接退出循环；
			if (k == 0 )
			{
				printf("not found!\n");
				break;
			}

			//这是判断在找到一个要删除的数值后，查找数组中还有没有相同的数值，
			//如果有继续循环删除，如果没有就提示完成，退出循环；
			if (j > i )
			{
				printf("done!\n");
				break;
			}
			for(; j <= i-1; j++)
			{
				if(j == 9)	//判断j是否为数组的第10个数值，
				{
					arr[j] = 0;
					continue;
				}	
				arr[j] = arr[j+1];
			}
			i--;
		}

	}
*/
