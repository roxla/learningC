#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 声明一个单向不循环链表
typedef struct sl_list
{
    // 1.数据域
    int data;

    // 2.指针域
    struct sl_list *next;
} node_st, *node_pt;

// 初始化单向链表
node_pt link_list_init(void);
// 打印链表所有数据（遍历）
void link_list_show(node_pt head);
// 添加数据到链表
int link_list_add(int new_data, node_pt head);
// 删除指定的数据节点
int link_list_del(int del_data, node_pt head);

int main(int argc, char const *argv[])
{
    // 1.初始化单向链表
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
            link_list_add(num, head);
        }
        else if (num < 0)
        {
            // 删除该数据
            while (link_list_del(-num, head))
                link_list_del(-num, head);
        }
        else
        {
            node_pt pos = head->next;
            while (1)
            {
                printf("%d\n", pos->data);
                pos = pos->next;
                if(pos == head)
                    pos = pos->next;
                sleep(1);
            }
        }
        // 显示链表所有数据
        link_list_show(head);
    }
    return 0;
}

// 初始化单向循环链表
node_pt link_list_init(void)
{
    // a. 头节点申请一个堆空间
    node_pt head = malloc(sizeof(node_st));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }
    // b. 让头节点的指针域next指向NULL
    head->next = head;

    // c.将申请的堆空间返回
    return head;
}

// 添加数据到链表
int link_list_add(int new_data, node_pt head)
{
    // a. 给新节点分配一个堆空间
    node_pt new_node = malloc(sizeof(node_st));
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
int link_list_del(int del_data, node_pt head)
{
    // A. 判断是否为空链
    if (head->next == head)
    {
        printf("Empty!");
        return 0;
    }
    // B. 遍历链表，找出待删除数据的地址（需要两个指针同步移动），记录
    node_pt pos = head->next, pos_prev = head;
    for (; pos != head; pos_prev = pos, pos = pos->next)
    {
        if (pos->data == del_data)
            break;
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (pos == head)
        return 0;
    // C. 修改指向即可
    // 让待删除节点的前节点的指针域，指向后节点。
    pos_prev->next = pos->next;
    // D.释放待删除节点的堆空间
    free(pos);

    return 1;
}

// 打印链表所有数据（遍历）
void link_list_show(node_pt head)
{
    printf("=========DATA========\n");
    node_pt pos;
    for (pos = head->next; pos != head; pos = pos->next)
        printf("%d ", pos->data);
    printf("\n");
}
