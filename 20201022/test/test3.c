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
// 打印链表所有数据（前序遍历）
void link_list_show_prev(node_pt head);
// 添加数据到链表头
int link_list_add(int new_data, node_pt head);
// 添加数据到链表尾
int link_list_add_tail(int new_data, node_pt head);
// 删除指定的数据节点
int link_list_del(int new_data, node_pt head);

int main(int argc, char const *argv[])
{
    // 1.初始化双向循环链表
    node_pt head = link_list_init();

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
            // link_list_add(num, head);
            link_list_add_tail(num, head);
        }
        else if (num < 0)
        {
            // 删除该数据
            while (link_list_del(-num, head))
                link_list_del(-num, head);
        }
        else
        {
            // 退出程序
            return 1;
        }
        // 显示链表所有数据（代码与单向循环链表是一样的）
        link_list_show_next(head);
        // link_list_show_prev(head);
    }
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

// 打印链表所有数据（后序遍历）
void link_list_show_next(node_pt head)
{
    printf("=========后序DATA========\n");
    node_pt pos;
    for (pos = head->next; pos != head; pos = pos->next)
        printf("%d ", pos->data);
    printf("\n");
}

// 打印链表所有数据（前序遍历）
void link_list_show_prev(node_pt head)
{
    printf("=========前序DATA========\n");
    node_pt pos;
    for (pos = head->prev; pos != head; pos = pos->prev)
        printf("%d ", pos->data);
    printf("\n");
}

// 添加数据到链表头
int link_list_add(int new_data, node_pt head)
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
    // 3.1 修改新节点指向，prev指前节点，next指后节点
    new_node->prev = head;
    new_node->next = head->next;

    // 3.2 修改前后节点指向，前节点和后节点都一起指向新节点
    // （顺序不能调换，否则不能通过head访问到后节点了）
    head->next->prev = new_node;
    head->next = new_node;
    // new_node->next->prev = new_node;	//通过新节点访问后节点

    return 0;
}

// 添加数据到链表尾
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

// 删除指定的数据节点（后序删除）
int link_list_del(int new_data, node_pt head)
{
    if (head->next == head)
    {
        printf("Empty!");
        return 0;
    }
    node_pt pos;
    for (pos = head->next; pos != head; pos = pos->next)
    {
        if (pos->data == new_data)
            break;
    }
    if (pos == head)
        return 0;
    pos->next->prev = pos->prev;
    pos->prev->next = pos->next;
    free(pos);

    return 1;
}