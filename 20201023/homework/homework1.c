#include "list.h" // 默认在当前目录查找头文件，或者在系统目录查找
#include <stdio.h>
#include <stdlib.h> // 默认在系统目录查找头文件

// 声明一个内核链表节点结构体（俗称：大结构体）
typedef struct kernel_list
{
    // 1.数据域
    int data;

    // 2.指针域（俗称：小结构体）
    struct list_head list; //（前后关系）
} kl_st, *kl_pt;

// 初始化内核链表
kl_pt kl_list_init(void);
// 打印链表所有数据（后序遍历）
void kl_list_show(kl_pt head);
// 打印链表所有数据（前序遍历）
void kl_list_show_prev(kl_pt head);
// 添加数据到链表头
int kl_list_add(int new_data, kl_pt head);
// 添加数据到链表尾
int kl_list_add_tail(int new_data, kl_pt head);
// 删除指定的数据节点
int kl_list_del(int del_data, kl_pt head);
// 有序添加数据到链表中
int kl_list_add_ans(int new_data, kl_pt head);

int main(int argc, char const *argv[])
{
    // 1.初始化一条内核链表（头节点）
    kl_pt head = kl_list_init();

    // 2.操作
    int n;
    while (1)
    {
        fprintf(stderr, "Pls Input: ");
        scanf("%d", &n);
        while (getchar() != '\n')
            ;

        if (n > 0)
        {
            //添加
            // kl_list_add(n, head);
            // kl_list_add_tail(n, head);
            kl_list_add_ans(n, head);
        }
        else if (n < 0)
        {
            //删除
            kl_list_del(-n, head);
        }
        else
        {
            //退出
            printf("Bye Bye~\n");
            return 1;
        }
        kl_list_show(head);
        kl_list_show_prev(head);
    }

    return 0;
}

// 有序添加数据到链表中
int kl_list_add_ans(int new_data, kl_pt head)
{
    // 1.申请堆空间给新节点
    kl_pt new_node = malloc(sizeof(kl_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // 2.将数据给入新节点的数据域
    new_node->data = new_data;

    // 3.逐个遍历找出比新数据更大的数据节点，记录get_node
    kl_pt get_node, n;
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        if (get_node->data > new_data)
            break;
    }

    // 4.将新节点插入到 get_node 之前
    list_add_tail(&new_node->list, &get_node->list);

    /*
	// 3.逐个遍历找出比新数据更大的数据节点，记录pos
	kl_pt get_node;
	struct list_head *pos, *n;
	list_for_each_safe(pos, n, &head->list)
	{
		get_node = list_entry(pos, kl_st, list);
		if(get_node->data > new_data)
			break;
	}

	// 4.将新节点插入到 pos 之前
	list_add_tail(&new_node->list, pos);
	*/

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

// 删除指定的数据节点
int kl_list_del(int del_data, kl_pt head)
{
    // 1.判断是否为空链
    if (list_empty(&head->list))
    {
        printf("Empty!\n");
        return 1;
    }

    // 2.遍历链表，逐个对比查找对应的数据，记录pos
    kl_pt get_node;        //遍历大结构体
    struct list_head *pos; //遍历小结构体
    struct list_head *n;   //临时指针变量
    list_for_each_safe(pos, n, &head->list)
    {
        get_node = list_entry(pos, kl_st, list);
        if (get_node->data == del_data)
            break;
    }

    //没找到，提示并退出函数
    if (pos == &head->list)
    {
        printf("Not Found\n");
        return 2;
    }

    // 3.修改指向
    list_del(pos);

    // 4.释放堆空间
    free(get_node);

    return 0;
}

// 添加数据到链表尾
int kl_list_add_tail(int new_data, kl_pt head)
{
    // 1.申请堆空间给新节点
    kl_pt new_node = malloc(sizeof(kl_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // 2.将数据给入新节点的数据域
    new_node->data = new_data;

    // 3.修改指向
    list_add_tail(&new_node->list, &head->list);

    return 0;
}

// 打印链表所有数据（前序遍历）
void kl_list_show_prev(kl_pt head)
{
    kl_pt get_node;

    // 从第一个节点遍历所有的节点，直到head
    printf("==========前序===========\n");
    struct list_head *pos;
    list_for_each_prev(pos, &head->list)
    {
        get_node = list_entry(pos, kl_st, list);
        printf("%d ", get_node->data);
    }
    printf("\n");
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
        printf("%d ", get_node->data);
    }
    printf("\n");
}
