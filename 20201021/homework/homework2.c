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
// 删除指定的数据节点
int link_list_del(int del_data, struct sl_list *head);
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

// 删除指定的数据节点
int link_list_del(int del_data, struct sl_list *head)
{
    // A. 判断是否为空链
    if (head->next == NULL)
    {
        printf("Empty!");
        return 0;
    }
    // B. 遍历链表，找出待删除数据的地址（需要两个指针同步移动），记录
    struct sl_list *pos = head->next, *pos_prev = head;
    for (; pos != NULL; pos_prev = pos, pos = pos->next)
    {
        if (pos->data == del_data)
            break;
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (pos == NULL)
        return 0;
    // C. 修改指向即可
    // 让待删除节点的前节点的指针域，指向后节点。
    pos_prev->next = pos->next;
    // D.释放待删除节点的堆空间
    free(pos);

    return 1;
}

// 翻转链表
void turn_list(struct sl_list *head)
{
    struct sl_list *end_node, *pos, *pos_temp;
    end_node = head;
    while (end_node->next)
        end_node = end_node->next;

    for (pos = head->next; pos != end_node; pos = pos_temp)
    {
        // （移动：删除节点+添加新节点）
        //（温馨提示：需要记录pos的后节点地址，否则被释放了不能访问了）
        // 直到移动到pos_tail位置，停止
        pos_temp = pos->next; // 记录后一个节点地址（指针域）
        // 插入节点到链表尾(头插法)
        link_list_add(pos->data, end_node);
        // 删除链表头后面的节点
        link_list_del(pos->data, head);
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