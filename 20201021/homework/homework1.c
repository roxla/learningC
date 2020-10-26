#include <stdio.h>
#include <stdlib.h>

// 声明一个单向不循环链表
struct sl_list
{
    // 1.数据域
    int data;

    // 2.指针域
    struct sl_list *next;
};

// 初始化单向链表
struct sl_list *link_list_init(void);
// 添加数据到链表
int link_list_add(int new_data, struct sl_list *head);
// 翻转链表
void turn_list(struct sl_list *head);
// 打印链表所有数据（遍历）
void link_list_show(struct sl_list *head);

int main(int argc, char const *argv[])
{
    // 1.初始化单向链表
    struct sl_list *head = link_list_init();

    // 2.操作
    int num;
    while (1)
    {
        fprintf(stderr, "Pls Input: ");
        scanf("%d", &num);
        while (getchar() != '\n')
            ;
        if (num > 0)
        {
            // 插入该数据
            link_list_add(num, head);
        }
        else
        {
            turn_list(head);
        }

        // 显示链表所有数据
        link_list_show(head);
    }

    return 0;
}

// 初始化单向链表
struct sl_list *link_list_init(void)
{
    // a. 头节点申请一个堆空间
    struct sl_list *head = malloc(sizeof(struct sl_list));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }
    // b. 让头节点的指针域next指向NULL
    head->next = NULL;

    // c.将申请的堆空间返回
    return head;
}

// 添加数据到链表
int link_list_add(int new_data, struct sl_list *head)
{
    // a. 给新节点分配一个堆空间
    struct sl_list *new_node = malloc(sizeof(struct sl_list));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // b. 把数据给入新节点数据域
    new_node->data = new_data;

    // c. 修改指针（不能调换）
    // 	1. 把头节点的指针域, 赋值给新节点的指针域（偷过来）
    // 	（让新节点next指向头节点的next）
    new_node->next = head->next;

    // 	2. 把新节点的地址，赋值给头节点的指针域
    // 	（让头节点next指向新节点）
    head->next = new_node;

    return 0;
}

// 翻转链表
void turn_list(struct sl_list *head)
{
    struct sl_list *end_node, *pos;
    int tmp = 0, i = 0, j;
    end_node = head;
    while (end_node->next)
    {
        end_node = end_node->next;
        i++; // 链表总长度
    }
    for (; i > 0; i--)
    {
        pos = head->next;
        for (j = 1; j < i; j++) // 交换次数为总长度-1，每次交换的次数-1
        {
            tmp = pos->next->data;
            pos->next->data = pos->data;
            pos->data = tmp;

            pos = pos->next;
        }
    }
}

// 打印链表所有数据（遍历）
void link_list_show(struct sl_list *head)
{
    printf("=========DATA========\n");
    struct sl_list *pos;
    for (pos = head->next; pos != NULL; pos = pos->next)
        printf("%d ", pos->data);
    printf("\n");
}