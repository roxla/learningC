#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 声明一个双向链表
typedef struct sc_list
{
    // 1.数据域
    int data;

    // 2.指针域
    struct sc_list *prev; // 上一个节点
    struct sc_list *next; //下一个节点

} node_st, *node_pt;

// 初始化双向链表
node_pt link_list_init(void);
// 添加数据到链表头
int link_list_add(int new_data, node_pt head);
// 打印链表所有数据（遍历）
void link_list_show(node_pt head);

int main(int argc, char const *argv[])
{
    node_pt head = link_list_init();
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
        else if (num < 0)
        {
            // 删除该数据
        }
        else
        {
        }
        // 显示链表所有数据
        link_list_show(head);
    }
    return 0;

    return 0;
}

node_pt link_list_init(void)
{
    // a. 头节点申请一个堆空间
    node_pt head = malloc(sizeof(node_st));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }
    head->next = NULL;
    head->prev = NULL;

    return head;
}

int link_list_add(int new_data, node_pt head)
{
    node_pt new_node = malloc(sizeof(node_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    new_node->data = new_data;

    // 头节点的地址赋值给新节点的上一节点指针域
    new_node->prev = head;
    // 头节点的的下一节点地址赋值给新节点的下一节点指针域
    new_node->next = head->next;
    // 新节点的地址赋值给头节点的的下一节点指针域
    head->next = new_node;
    if (new_node->next)
        new_node->next->prev = new_node;

    return 0;
}

// 打印链表所有数据（遍历）
void link_list_show(node_pt head)
{
    printf("=========DATA========\n");
    node_pt pos;
    for (pos = head->next; pos != NULL; pos = pos->next)
    {
        printf("%d ", pos->data);
        printf("prev: %p ", pos->prev);
        printf("next: %p ", pos->next);
        printf("node: %p\n", pos);
    }
    printf("\n");
}