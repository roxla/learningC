#include "my.h"

// 初始化内核链表
ebm_pt ebm_list_init(void)
{
    // 申请一块堆空间给头节点
    ebm_pt head = malloc(sizeof(ebm_st));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }

    // 修改头节点的指针域，使它们都指向自身
    INIT_LIST_HEAD(&head->list);

    // 将堆空间首地址返回
    return head;
}

// 添加数据到链表尾
int ebm_list_add_tail(char *name, char *path, char *path_name, long n_size, long n_date, long name_size, ebm_pt head)
{
    // 申请堆空间给新节点
    ebm_pt new_node = malloc(sizeof(ebm_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }

    // 将数据给入新节点的数据域
    strncpy(new_node->name, name, sizeof(new_node->name));
    strncpy(new_node->path, path, sizeof(new_node->path));
    strncat(new_node->path_name, path_name, sizeof(new_node->path_name));
    new_node->size = n_size;
    new_node->date = n_date;
    new_node->name_size = name_size;
    // 修改指向
    list_add_tail(&new_node->list, &head->list);

    return 0;
}

// 打印链表所有数据（后序遍历）
void ebm_list_show(ebm_pt head)
{
    ebm_pt get_node, n;
    // 从第一个节点遍历所有的节点，直到head
    printf("==========后序===========\n");
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        printf("%s ", get_node->name);
        printf("%ld ", get_node->name_size);
        printf("%s ", get_node->path);
        printf("%s ", get_node->path_name);
        printf("%ld ", get_node->size);
        printf("%ld ", get_node->date);
        printf("\n");
    }
}