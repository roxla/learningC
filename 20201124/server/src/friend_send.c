#include "head.h"

void friend_send_main(pt_uinfo user)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/friend";
    int num, loop = 1;
    char user_info[100];
    snprintf(user_info, sizeof(user_info), "当前用户:%s    是否为vip:%d\n", user->name, user->vip);
    // 功能选项
    while (loop)
    {
        send(user->connfd, user_info, strlen(user_info), 0);
        while (send_view(user->connfd, viwe_path))
            ;
        num = num_msg(user->connfd);
        switch (num)
        {
        case 1: // 好友聊天
            friend_send(user);
            break;
        case 2: // 好友列表
            friend_list(user);
            break;
        case 3: // 添加好友
            add_friend_list(user);
            break;
        case 4: // 删除好友
            del_friend_list(user);
            break;
        default: // 返回
            return;
            break;
        }
    }
}

void friend_send(pt_uinfo user)
{
    char *tmp = NULL;
    char tips[] = "Pls input msg: \n";
    // 将聊天室状态置为1
    user->istalk = 1;
    // 群发上线消息
    char msg[MAX_IO], buf[MAX_IO];
    snprintf(msg, sizeof(msg), "inline!");
    friend_msg_broadcast(user, msg);
    while (1)
    {
        send(user->connfd, tips, strlen(tips), 0);
        bzero(msg, sizeof(msg));
        recv(user->connfd, msg, sizeof(msg), 0);
        if (strncmp(msg, "quit", 4) == 0)
        {
            // 将聊天室状态置为0
            user->istalk = 0;
            // 群发下线消息
            snprintf(msg, sizeof(msg), "outline!");
            friend_msg_broadcast(user, msg);
            // 退出
            return;
        }

        tmp = strstr(msg, ":");
        if (tmp == NULL) // 说明没有找到: 说明是群发
        {
            friend_msg_broadcast(user, msg);
        }
        else // 找到了: 说明是私聊
        {
            friend_msg_send(user, strtok(msg, ":"), tmp);
        }
    }
}

// 群发消息
int friend_msg_broadcast(pt_uinfo user, const char *msg)
{
    pt_uinfo get_node, n;
    pt_friend fri_pos;
    char buf[MAX_IO + NAME_MAX_LEN + 10];
    for (fri_pos = user->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
    {
        list_for_each_entry_safe(get_node, n, &boss->head->list, list)
        {
            if (!strcmp(get_node->name, fri_pos->my_friend))
            {
                if (get_node->istalk == 0 || get_node->connfd == -1)
                {
                    continue;
                }
                snprintf(buf, sizeof(buf), "%s: %s\n", user->name, msg);
                send(get_node->connfd, buf, strlen(buf), 0);
            }
        }
    }

    return 0;
}

// 私聊
int friend_msg_send(pt_uinfo user, const char *name, const char *msg)
{
    pt_uinfo get_node, n;
    pt_friend fri_pos;
    char buf[MAX_IO + NAME_MAX_LEN + 10];
    for (fri_pos = user->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
    {
        if (!strcmp(name, fri_pos->my_friend))
            break;
    }
    if (fri_pos)
    {
        list_for_each_entry_safe(get_node, n, &boss->head->list, list)
        {
            if (!strcmp(get_node->name, fri_pos->my_friend))
            {
                if (get_node->istalk == 0 || get_node->connfd == -1)
                {
                    snprintf(buf, sizeof(buf), "%s未在线\n", name);
                    send(user->connfd, buf, strlen(buf), 0);
                }
                else
                {
                    snprintf(buf, sizeof(buf), "To %s: %s\n", user->name, msg + 1);
                    send(get_node->connfd, buf, strlen(buf), 0);
                }
                break;
            }
        }
    }
    else
    {
        snprintf(buf, sizeof(buf), "%s不是你的好友,请先添加好友\n", name);
        send(user->connfd, buf, strlen(buf), 0);
    }

    return 0;
}