#include "head.h"

// 显示好友列表
void friend_list(pt_uinfo user)
{
    pt_uinfo get_node, n;
    pt_friend fri_pos;

    char list_view[100];
    char logtype[10]; // 是否登录
    char litype[10];  // 是否在线

    snprintf(list_view, sizeof(list_view), "\n===================== 好友列表 =======================\n");
    send(user->connfd, list_view, strlen(list_view), 0);
    for (fri_pos = user->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
    {
        list_for_each_entry_safe(get_node, n, &boss->head->list, list)
        {
            if (!strcmp(get_node->name, fri_pos->my_friend))
            {
                if (get_node->connfd == -1)
                {
                    snprintf(logtype, sizeof(logtype), "未登录");
                    snprintf(litype, sizeof(litype), "未连接");
                }
                else
                {
                    snprintf(logtype, sizeof(logtype), "已登录");
                    if (get_node->istalk == 0)
                    {
                        snprintf(litype, sizeof(litype), "未连接");
                    }
                    else
                    {
                        snprintf(litype, sizeof(litype), "已连接");
                    }
                }
                snprintf(list_view, sizeof(list_view), "  %s >>>>> %s >>>>> %s\n", get_node->name, logtype, litype);
                send(user->connfd, list_view, strlen(list_view), 0);
            }
        }
    }
    snprintf(list_view, sizeof(list_view), "======================================================\n");
    send(user->connfd, list_view, strlen(list_view), 0);
}

// 添加好友
void add_friend_list(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;

    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入要添加的用户名:\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        else
        {
            break;
        }
    }
    // 判断是不是自己
    if (!strncmp(user->name, buf, strlen(buf) - 1) && strlen(user->name) == (strlen(buf) - 1))
    {
        snprintf(tip_cli, sizeof(tip_cli), "不能添加自己为好友!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    pt_uinfo get_node, n;
    pt_friend fri_pos;
    // 判断是否已有好友
    for (fri_pos = user->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
    {
        if (!strncmp(fri_pos->my_friend, buf, strlen(buf) - 1) && strlen(fri_pos->my_friend) == (strlen(buf) - 1))
        {
            snprintf(tip_cli, sizeof(tip_cli), "已和该用户是好友!\n");
            // 发送提示
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
    }
    int flag = 0;
    // 查找该用户
    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {
        if (!strncmp(get_node->name, buf, strlen(buf) - 1) && strlen(get_node->name) == (strlen(buf) - 1))
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        // 自己添加
        add_friend_tail(user->myFriend, get_node->name);
        // 对方添加
        add_friend_tail(get_node->myFriend, user->name);
        snprintf(tip_cli, sizeof(tip_cli), "已添加该好友!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        if (get_node->connfd != -1)
        {
            snprintf(tip_cli, sizeof(tip_cli), "%s已添加你为好友!\n", user->name);
            // 发送提示
            send(get_node->connfd, tip_cli, strlen(tip_cli), 0);
        }
        return;
    }
    else
    {
        snprintf(tip_cli, sizeof(tip_cli), "未找到该用户!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
}

// 删除好友
void del_friend_list(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;

    // 判断是否为空链
    if (user->myFriend->next == NULL)
    {
        snprintf(tip_cli, sizeof(tip_cli), "你已没有好友!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 获取数据
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入要删除的用户名:\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        else
        {
            break;
        }
    }
    // 判断是不是自己
    if (!strncmp(user->name, buf, strlen(buf) - 1) && strlen(user->name) == (strlen(buf) - 1))
    {
        snprintf(tip_cli, sizeof(tip_cli), "不能删除自己!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 查找好友列表
    // 遍历链表，找出待删除数据的地址（需要两个指针同步移动），记录
    pt_friend fri_pos = user->myFriend->next;
    pt_friend fri_pos_prev = user->myFriend;
    printf("user->myFriend: %p\n", user->myFriend);
    for (; fri_pos != NULL; fri_pos_prev = fri_pos, fri_pos = fri_pos->next)
    {
        if (!strncmp(fri_pos->my_friend, buf, strlen(buf) - 1) && strlen(fri_pos->my_friend) == (strlen(buf) - 1))
        {
            break;
        }
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (fri_pos == NULL)
    {
        snprintf(tip_cli, sizeof(tip_cli), "未找到该用户!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 让待删除节点的前节点的指针域，指向后节点
    fri_pos_prev->next = fri_pos->next;
    // 释放待删除节点的堆空间
    free(fri_pos);

    // 查找该用户
    int flag = 0;
    pt_uinfo get_node, n;

    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {

        if (!strncmp(get_node->name, buf, strlen(buf) - 1) && strlen(get_node->name) == (strlen(buf) - 1))
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        fri_pos = get_node->myFriend->next;
        fri_pos_prev = get_node->myFriend;
        for (; fri_pos != NULL; fri_pos_prev = fri_pos, fri_pos = fri_pos->next)
        {
            if (!strcmp(fri_pos->my_friend, user->name))
            {
                break;
            }
        }
        if (fri_pos == NULL)
        {
            snprintf(tip_cli, sizeof(tip_cli), "未找到该用户!\n");
            // 发送提示
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
        // 删除该用户
        fri_pos_prev->next = fri_pos->next;
        free(fri_pos);
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "已删除该好友!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
    }
    else
    {
        snprintf(tip_cli, sizeof(tip_cli), "未找到该用户!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
    }

    // 打印看看
    user_info_list_show(boss->head);
}