#include <stdio.h>
#include <stdlib.h> // 默认在系统目录查找头文件
#include <unistd.h>

#include "list.h" // 默认在当前目录查找头文件，或者在系统目录查找

typedef struct kernel_list
{
    // 1.数据域
    int data;

    // 2.指针域（俗称：小结构体）
    struct list_head list; //（前后关系）
} kl_st, *kl_pt;

// 初始化内核链表
kl_pt kl_list_init(void);
//

// 打印链表所有数据（后序遍历）
void kl_list_show(kl_pt head);
// 添加数据到链表头
int kl_list_add(int new_data, kl_pt head);

int main(int argc, char const *argv[])
{
    // 1.初始化一条内核链表（头节点）
    kl_pt head = kl_list_init();

    // 2.输入一个整数n，形成一条内核链表，并打印
    int n;
    while (1)
    {
        fprintf(stderr, "Pls Input: ");
        scanf("%d", &n);
        while (getchar() != '\n')
            ;
        if (n >= 0)
        {
            kl_list_add(n, head);
        }
        else
        {
            
        }
        kl_list_show(head);
    }

    return 0;
}

// 初始化内核链表头节点
kl_pt kl_list_init(void)
{
    // 1.申请一块堆空间给头节点
    kl_pt head = malloc(sizeof(kl_st));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }

    // 2.修改头节点的指针域，使它们都指向自身
    // head->list.prev = &(head->list);
    // head->list.next = &head->list;
    INIT_LIST_HEAD(&head->list);
    //head指的是大结构体，这句话的意思就是大结构体head指向小结构体找出小结构体的地址
    // 3.将堆空间首地址返回
    return head;
}

// 添加数据到链表头
int kl_list_add(int new_data, kl_pt head)
{
    // 1.给新节点申请一块堆空间
    kl_pt new_node = malloc(sizeof(kl_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // 2.将新数据给入数据域
    new_node->data = new_data;

    // 3.修改指向
    list_add(&new_node->list, &head->list);
    //new_node->list：新节点   head->list头节点

    return 0;
}

// 打印链表所有数据（后序遍历）
void kl_list_show(kl_pt head)
{
    kl_pt get_node;

    // 从第一个节点遍历所有的节点，直到head
    printf("==========后序===========\n");
    struct list_head *pos;
    list_for_each(pos, &head->list)
    {
        get_node = list_entry(pos, kl_st, list);
        printf("%d<--->", get_node->data);
    }
    printf("\n");
}