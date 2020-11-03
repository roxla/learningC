#include "my.h"

void ascending_order_name(ebm_pt head);
// 按时间升序
void ascending_order_time(ebm_pt head);
// 按名称降序
void descending_order_name(ebm_pt head);
// 按时间降序
void descending_order_time(ebm_pt head);

void rearrange(int mode, const char *type, ebm_pt head)
{
    // mode: 1 升序， 2 降序
    // type: time 按时间，name 按大小
    if (!strcmp("name", type))
    {
        if (mode == 1)
        {
            ascending_order_name(head);
        }
        else if (mode == 2)
        {
            descending_order_name(head);
        }
    }
    else if (!strcmp("time", type))
    {
        if (mode == 1)
        {
            ascending_order_time(head);
        }
        else if (mode == 2)
        {
            descending_order_time(head);
        }
    }
}

// 按名称升序
void ascending_order_name(ebm_pt head)
{
    // 遍历计算链表节点个数
    int n = 0;
    struct list_head *pos;
    list_for_each(pos, &head->list)
        n++;
    // 开始遍历（次数i: n-1）
    int change_flag; // 交换标志位
    ebm_pt get_node1, get_node2;
    int i, j;
    for (i = 1; i <= n - 1; i++)
    {
        // 每轮遍历将标志位置0
        change_flag = 0;
        // 使得pos指向首个数据节点
        pos = head->list.next;
        // 两两对比（次数j：n-i）
        for (j = 0; j < n - i; j++)
        {
            // 如果左数大于右数
            get_node1 = list_entry(pos, ebm_st, list);
            get_node2 = list_entry(pos->next, ebm_st, list);

            if (get_node1->name_size > get_node2->name_size)
            {
                // 交换位置, pos保持不动
                // list_move(左数节点1, 右数节点2);
                // list_move_tail(右数节点2, 左数节点1);
                list_move(pos, pos->next);

                change_flag = 1;
            }
            else
            {
                // pos需要向后移动一位
                pos = pos->next;
            }
        }
        // 如果该标志位仍然为0，说明未发生交换动作，跳出
        if (change_flag == 0)
            break;
    }

    return;
}

// 按时间升序
void ascending_order_time(ebm_pt head)
{
    // 遍历计算链表节点个数
    int n = 0;
    struct list_head *pos;
    list_for_each(pos, &head->list)
        n++;
    // 开始遍历（次数i: n-1）
    int change_flag; // 交换标志位
    ebm_pt get_node1, get_node2;
    int i, j;
    for (i = 1; i <= n - 1; i++)
    {
        // 每轮遍历将标志位置0
        change_flag = 0;
        // 使得pos指向首个数据节点
        pos = head->list.next;
        // 两两对比（次数j：n-i）
        for (j = 0; j < n - i; j++)
        {
            // 如果左数大于右数
            get_node1 = list_entry(pos, ebm_st, list);
            get_node2 = list_entry(pos->next, ebm_st, list);

            if (get_node1->date > get_node2->date)
            {
                // 交换位置, pos保持不动
                // list_move(左数节点1, 右数节点2);
                // list_move_tail(右数节点2, 左数节点1);
                list_move(pos, pos->next);

                change_flag = 1;
            }
            else
            {
                // pos需要向后移动一位
                pos = pos->next;
            }
        }
        // 如果该标志位仍然为0，说明未发生交换动作，跳出
        if (change_flag == 0)
            break;
    }
    return;
}

// 按名称降序
void descending_order_name(ebm_pt head)
{
    // 取链表首元素作为基准点
    // head->list.next;
    // 将小于基准点的元素放在前方，把大于基准点的元素放在后方
    // 记一个游标i，指向待排序数组的首位，它将会不断向后移动
    // 再记一个游标j，指向待排序数组的末位，它将会不断向前移动
    // 现在让游标j从后往前扫描，寻找比基准点小的元素x，找到后停下来，把这个元素扔到前方去
    // 让游标i向后扫描，寻找比枢轴大的元素y，找到后停下来，与pivot进行交换
    return;
}

// 按时间降序
void descending_order_time(ebm_pt head)
{
    return;
}
