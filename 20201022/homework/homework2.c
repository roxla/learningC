#include <stdio.h>
#include <stdlib.h>

// 声明一个双向循环链表
typedef struct dc_list
{
    // 1.数据域
    int data;

    // 2.指针域
    struct dc_list *next; //下节点地址
    struct dc_list *prev; //上节点地址
} node_st, *node_pt;

// 初始化双向循环链表
node_pt link_list_init(void);
// 打印链表所有数据（后序遍历）
void link_list_show_next(node_pt head);
// 添加数据到链表尾
int link_list_add_tail(int new_data, node_pt head);
// 规则排序
int link_list_sort(node_pt head, int num);

int main(int argc, char const *argv[])
{
    // 1.初始化双向循环链表
    node_pt head = link_list_init();

    // 2.操作
    int num, i;
    fprintf(stderr, "Pls Input: ");
    scanf("%d", &num);
    while (getchar() != '\n')
        ;
    for (i = 1; i <= num; i++)
        link_list_add_tail(i, head);
    link_list_sort(head, num);
    link_list_show_next(head);

    return 0;
}

// 初始化双向循环链表
node_pt link_list_init(void)
{
    // 1.申请一个堆空间给头节点
    node_pt head = malloc(sizeof(node_st));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }

    // 2.修改指针域，使他们都指向本身
    head->prev = head;
    head->next = head;

    // 3.将堆空间返回
    return head;
}

// 添加数据到链表尾(插入到链表最末尾)
int link_list_add_tail(int new_data, node_pt head)
{
    // 1.申请堆空间给新节点
    node_pt new_node = malloc(sizeof(node_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // 2.将数据给入新节点的数据域
    new_node->data = new_data;

    // 3.修改指向
    // 3.1 操作新节点，prev指前节点(尾)，next指向后节点(头)
    new_node->prev = head->prev;
    new_node->next = head;
    // 3.2 操作前后节点，前节点(尾)next，和后节点(头)prev都指向新节点
    head->prev->next = new_node;
    head->prev = new_node;

    return 0;
}

// 打印链表所有数据（后序遍历）
void link_list_show_next(node_pt head)
{
    printf("=========后序DATA========\n");
    node_pt pos;
    for (pos = head->next; pos != head; pos = pos->next)
        printf("%d ", pos->data);
    printf("\n");
}

// 规则排序
int link_list_sort(node_pt head, int num)
{
    if (num < 3)
    {
        printf("Empty!");
        return 0;
    }
    node_pt pos;
    int i = 0;
    for (pos = head->next; pos != head; pos = pos->next)
    {
        if (pos->data % 2 == 0)
        {
            i += 1;
            pos->next->prev = pos->prev;
            pos->prev->next = pos->next;
            free(pos);
        }
    }
    for (; i > 0;i--)
        link_list_add_tail(i * 2, head);

    return 1;
}