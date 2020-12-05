#include "head.h"

// 释放用户管理结构体
void boss_list_free()
{
    pt_uinfo get_node, n;
    int connfd;
    pt_friend fri_pos, fri_prev;
    pt_myflg flg_pos, flg_prev;
    pt_twitter twi_pos, twi_prev;
    // 循环释放所有节点
    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {
        // 释放堆空间
        if (get_node->myFriend->next) // 判断是否为空链
        {
            fri_pos = get_node->myFriend->next;
            fri_prev = get_node->myFriend;
            for (; fri_pos != NULL; fri_pos = fri_pos->next)
            {
                fri_prev->next = fri_pos->next;
                free(fri_pos);
            }
        }
        free(get_node->myFriend); // 释放头
        printf("friend list free!\n");
        // 释放堆空间
        if (get_node->myflg->next) // 判断是否为空链
        {
            flg_pos = get_node->myflg->next;
            flg_prev = get_node->myflg;
            for (; flg_pos != NULL; flg_pos = flg_pos->next)
            {
                flg_prev->next = flg_pos->next;
                free(flg_pos);
            }
        }
        free(get_node->myflg); // 释放头
        printf("flight list free!\n");
        // 释放堆空间
        if (get_node->myTwitter->next) // 判断是否为空链
        {
            twi_pos = get_node->myTwitter->next;
            twi_prev = get_node->myTwitter;
            for (; twi_pos != NULL; twi_pos = twi_pos->next)
            {
                twi_prev->next = twi_pos->next;
                free(twi_pos);
            }
        }
        free(get_node->myTwitter); // 释放头
        printf("twitter list free!\n");
        printf("=====================================\n");
        if (get_node->connfd != -1)
        {
            connfd = get_node->connfd;
        }
        // 释放堆空间
        list_del(&get_node->list);
        free(get_node);

        send(connfd, "quit\n", 5, 0);
        close(connfd);
    }
    printf("all user exit\n");
    // 释放头
    free(boss->head);
    free(boss);
}

void plane_list_free()
{
    printf("free1\n");
    pt_flg get_node, n;
    // 循环释放所有节点
    list_for_each_entry_safe(get_node, n, &p_manager->head->list, list)
    {
        // 释放堆空间
        list_del(&get_node->list);
        free(get_node);
    }
    printf("free2\n");
    free(p_manager->head);
    free(p_manager);

    printf("all flight free\n");
}