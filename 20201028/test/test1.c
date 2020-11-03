#include <stdio.h>
#include <stdlib.h>

// 链式栈节点结构体
typedef struct stack_node
{
    // 1.数据域
    int data;

    // 2.指针域
    struct stack_node *next;
} node_st, *node_pt;

// 链式栈管理结构体
typedef struct stack_list
{
    // 栈顶节点地址
    struct stack_node *top;
} sl_st, *sl_pt;

// 初始化链式栈
sl_pt stack_list_init(void);
// 出栈
int stack_pop(sl_pt s);
// 入栈
void stack_push(int push_data, sl_pt s);

int main(int argc, char const *argv[])
{
    // 1.初始化一个空栈
    sl_pt s = stack_list_init();

    // 2.操作（正数入栈、负数出栈）
    int n;
    while (1)
    {
        fprintf(stderr, "Pls Input Dec: ");
        scanf("%d", &n);
        while (getchar() != '\n')
            ;

        // 计算8进制
        // 将余数逐个入栈
        while (1)
        {
            stack_push(n % 8, s);
            n /= 8;
            if (n == 0)
                break;
        }
        // 将链式栈中所有数据出栈
        printf("Oct: ");
        while (1)
        {
            n = stack_pop(s);
            if (n == -1)
                break;

            fprintf(stderr, "%d", n);
        }
        printf("\n");
    }

    return 0;
}

// 初始化链式栈
sl_pt stack_list_init(void)
{
    // 1.给管理结构体分配堆空间
    sl_pt s = malloc(sizeof(sl_st));
    if (s == NULL)
    {
        perror("sl_pt malloc failed");
        return NULL;
    }

    // 2.让栈顶节点地址指向NULL
    s->top = NULL;

    // 3.返回堆空间地址
    return s;
}

// 入栈
void stack_push(int push_data, sl_pt s)
{
    // 1. 给新节点分配堆空间，并把数据给入
    node_pt new_node = malloc(sizeof(node_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return;
    }
    new_node->data = push_data;

    // 2. 让新节点成为新的栈顶
    // 	a. 新节点的指针域next指向当前栈顶（旧）
    new_node->next = s->top;

    // 	b. 更新栈顶指针 *top
    s->top = new_node;
}

// 出栈
int stack_pop(sl_pt s)
{
    // 1. 判断是否为空栈
    if (s->top == NULL)
    {
        // printf("Empty!\n");
        return -1;
    }

    // 2. 将栈顶数据获取
    int temp_data = s->top->data;

    // 3. 定义temp临时保存栈顶地址（便于后续释放）
    node_pt temp = s->top;

    // 4. 更新栈顶指针 *top
    s->top = s->top->next;

    // 5. 释放temp
    free(temp);

    return temp_data;
}