#include "head.h"

// 已购航班列表
void user_flight_list(pt_uinfo user)
{
    // 结构头
    char utitle[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/user_flight/user_title";
    // 结构尾
    char ubtn[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/user_flight/flight_btn";
    // 结构分割线
    char uline[] = "-----------------------------------------------------------------------------\n";
    // 变量定义
    char plane[300];
    char cash[6];
    pt_flg get_node, n;
    pt_myflg flg_pos;
    // 发送头
    send_view(user->connfd, utitle);
    for (flg_pos = user->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
    {
        list_for_each_entry_safe(get_node, n, &p_manager->head->list, list)
        {
            if (!strncmp(flg_pos->flight, get_node->code, strlen(get_node->code)))
            {
                // 发送分割线
                send(user->connfd, uline, strlen(uline), 0);
                // 价格补0
                memset(cash, '\0', sizeof(cash));
                cash_cat(flg_pos->price, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
    }
    // 发送分割线
    send(user->connfd, uline, strlen(uline), 0);
    // 发送尾
    send_view(user->connfd, ubtn);
}
// 购买航班
void user_flight_buy(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    pt_flg flg_node, flg_n, flg_temp;
    // 获取航班号
    int flg1 = 0;
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入航班号:\n");
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
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, 5))
        {
            flg_temp = flg_node;
            flg1 = 1;
            break;
        }
    }
    // 判断
    if (!flg1)
    {
        snprintf(tip_cli, sizeof(tip_cli), "该航班不存在!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }

    // 获取身份证
    int flg2 = 0;
    pt_uinfo info_node, info_n, info_temp;
    pt_friend fri_pos;
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入自己或者好友的身份证号:\n");
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
    if (!strncmp(user->id, buf, strlen(buf) - 1) && strlen(user->id) == (strlen(buf) - 1))
    {
        flg2 = 1;
        info_temp = user;
    }
    else
    {
        for (fri_pos = user->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
        {
            list_for_each_entry_safe(info_node, info_n, &boss->head->list, list)
            {
                if (!strcmp(info_node->name, fri_pos->my_friend))
                {
                    if (!strncmp(info_node->id, buf, strlen(buf) - 1) && strlen(info_node->id) == (strlen(buf) - 1))
                    {
                        flg2 = 1;
                        info_temp = info_node;
                        break;
                    }
                }
            }
        }
    }
    // 判断
    if (!flg2)
    {
        snprintf(tip_cli, sizeof(tip_cli), "请输入正确的身份证号!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }

    // 判断是否已有该票
    pt_myflg flg_pos;
    int flg3 = 0;
    for (flg_pos = info_temp->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
    {
        if (!strcmp(flg_pos->flight, flg_temp->code))
        {
            flg3 = 1;
            snprintf(tip_cli, sizeof(tip_cli), "该账号已有该航班票!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            break;
        }
    }
    // 购票
    if (!flg3)
    {
        if (buy_tickets(info_temp, flg_temp, user))
        {
            snprintf(tip_cli, sizeof(tip_cli), "购票成功!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
        else
        {
            snprintf(tip_cli, sizeof(tip_cli), "购票失败!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
    }
}
// 购票
int buy_tickets(pt_uinfo temp, pt_flg pos, pt_uinfo user)
{
    char tip_cli[200];
    float off;
    int price;

    // 购票期间上锁
    pthread_mutex_lock(&m);

    if (pos->num <= 0)
    {
        snprintf(tip_cli, sizeof(tip_cli), "余票不足!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 解锁
        pthread_mutex_unlock(&m);

        return 0;
    }
    if (user->vip)
    {
        off = user->vip * 0.9 - (user->vip - 1);
        if (off <= 0.2)
        {
            off = 0.2;
        }
        snprintf(tip_cli, sizeof(tip_cli), "尊贵的 VIP%d 用户,您本次购票可享受 %.1f 折优惠\n", user->vip, off * 10);
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
    }
    else
    {
        off = 1;
    }
    price = pos->cash * off;
    printf("price: %d\n", price);
    if ((user->cash - price) < 0)
    {
        snprintf(tip_cli, sizeof(tip_cli), "账户余额不足,请充值!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 解锁
        pthread_mutex_unlock(&m);

        return 0;
    }
    else
    {
        user->cash -= price;
        pos->num--;
        // int add_flight_tail(pt_myflg head, cJSON *arrayItem)
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "plane", pos->code);
        cJSON_AddNumberToObject(item, "o_cost", pos->cash);
        cJSON_AddNumberToObject(item, "cost", price);
        add_flight_tail(temp->myflg, item);
        // 解锁
        pthread_mutex_unlock(&m);
        return 1;
    }
    //
    user_info_list_show(boss->head);
}
// 发送界面
void flight_manager(pt_uinfo user)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/quit";
    int num;
    // 功能选项
    while (1)
    {
        while (send_view(user->connfd, viwe_path))
            ;
        num = num_msg(user->connfd);
        switch (num)
        {
        case 1: // 退票
            user_plane_del(user);
            break;
        case 2: // 改签
            change_flight(user);
            break;
        default:
            return;
            break;
        }
    }
}
// 改签
void change_flight(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    // 判断是否为空链
    if (user->myflg->next == NULL)
    {
        snprintf(tip_cli, sizeof(tip_cli), "你当前没有航班!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 获取航班号
    snprintf(tip_cli, sizeof(tip_cli), "请输入要改签的航班号:\n");
    recv_fun(tip_cli, user->connfd, buf);
    // 判断是否有该票
    pt_myflg flg_pos, flg_pos_temp;
    for (flg_pos = user->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
    {
        if (!strncmp(buf, flg_pos->flight, 5))
        {
            flg_pos_temp = flg_pos;
            break;
        }
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (flg_pos == NULL)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "未找到该航班!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 查询航班信息
    pt_flg flg_node, flg_n, flg_temp1, flg_temp2;
    int isDelplane = 0;
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            flg_temp1 = flg_node;
            break;
        }
    }
    if (flg_node == p_manager->head)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "您可以改签为大于该航班售价300元的航班!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // flg_pos_temp
        isDelplane = 1; // 未找到票，使用用户信息改签
    }
    //
    snprintf(tip_cli, sizeof(tip_cli), "请输入改签的航班号:\n");
    recv_fun(tip_cli, user->connfd, buf);
    for (flg_pos = user->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
    {
        if (!strncmp(buf, flg_pos->flight, 5))
        {
            // 发送提示
            snprintf(tip_cli, sizeof(tip_cli), "不能改签为已有的的航班!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
    }
    printf("flight: %s\n", flg_pos_temp->flight);
    // 查询航班信息
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            flg_temp2 = flg_node;
            break;
        }
    }
    if (flg_node == p_manager->head)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "该航班不存在!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    if (flg_temp2->num <= 0)
    {
        snprintf(tip_cli, sizeof(tip_cli), "该航班余票为0!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    if (isDelplane)
    {
        if ((flg_temp2->cash + 300) > flg_pos_temp->o_price)
        {
            // 发送提示
            snprintf(tip_cli, sizeof(tip_cli), "该航班价格大于你的航班!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
        else
        {
            // 购票期间上锁
            pthread_mutex_lock(&m);

            memcpy(flg_pos_temp->flight, flg_temp2->code, strlen(flg_temp2->code));
            flg_temp2->num--;
            // 解锁
            pthread_mutex_unlock(&m);
            // 发送提示
            snprintf(tip_cli, sizeof(tip_cli), "改签成功!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
    else
    {
        if (flg_temp2->cash > flg_pos_temp->o_price)
        {
            // 发送提示
            snprintf(tip_cli, sizeof(tip_cli), "该航班价格大于你的航班!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            return;
        }
        else
        {
            // 购票期间上锁
            pthread_mutex_lock(&m);

            memcpy(flg_pos_temp->flight, flg_temp2->code, strlen(flg_temp2->code));
            flg_temp1->num++;
            flg_temp2->num--;
            // 解锁
            pthread_mutex_unlock(&m);
            // 发送提示
            snprintf(tip_cli, sizeof(tip_cli), "改签成功!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
}
// 退票
void user_plane_del(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    // 判断是否为空链
    if (user->myflg->next == NULL)
    {
        snprintf(tip_cli, sizeof(tip_cli), "你当前没有航班!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 获取航班号
    snprintf(tip_cli, sizeof(tip_cli), "请输入要退票的航班号:\n");
    recv_fun(tip_cli, user->connfd, buf);
    // 判断是否有该票
    pt_myflg flg_pos = user->myflg->next;
    pt_myflg flg_pos_prev = user->myflg;
    for (; flg_pos != NULL; flg_pos_prev = flg_pos, flg_pos = flg_pos->next)
    {
        if (!strncmp(buf, flg_pos->flight, 5))
        {
            break;
        }
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (flg_pos == NULL)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "未找到该航班!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 查询航班信息
    pt_flg flg_node, flg_n, flg_temp;
    int isDelplane = 0;
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            flg_temp = flg_node;
            break;
        }
    }
    if (flg_node == p_manager->head)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "您可以原价退回该票!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        isDelplane = 1;
    }
    // 退票
    // 上锁
    printf("cash: %d\n", user->cash);
    pthread_mutex_lock(&m);
    flg_temp->num++;
    if (isDelplane)
    {
        user->cash += flg_pos->price;
    }
    else
    {
        user->cash += (flg_pos->price * 0.8);
    }
    flg_pos_prev->next = flg_pos->next;
    free(flg_pos);
    // 解锁
    pthread_mutex_unlock(&m);
    printf("cash: %d\n", user->cash);

    snprintf(tip_cli, sizeof(tip_cli), "退票成功!\n");
    send(user->connfd, tip_cli, strlen(tip_cli), 0);
}
// 取票
void user_plane_get(pt_uinfo user)
{
    // 变量定义
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    // 判断是否为空链
    if (user->myflg->next == NULL)
    {
        snprintf(tip_cli, sizeof(tip_cli), "你当前没有航班!\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 获取航班号
    snprintf(tip_cli, sizeof(tip_cli), "请输入要取票的航班号:\n");
    recv_fun(tip_cli, user->connfd, buf);
    // 判断是否有该票
    pt_myflg flg_pos = user->myflg->next;
    pt_myflg flg_pos_prev = user->myflg;
    for (; flg_pos != NULL; flg_pos_prev = flg_pos, flg_pos = flg_pos->next)
    {
        if (!strncmp(buf, flg_pos->flight, 5))
        {
            break;
        }
    }
    // （如果遍历结束没找到，pos指向NULL，结束函数）
    if (flg_pos == NULL)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "未找到该航班!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 查询航班信息
    pt_flg flg_node, flg_n;
    int isDelplane = 0;
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            break;
        }
    }
    if (flg_node == p_manager->head)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "该航班已取消，请前往改签或退票!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // flg_pos_temp
        return;
    }
    // 出票
    // 结构头
    char utitle[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/plane_text/plane_title";
    // 结构尾
    char ubtn[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/plane_text/plane_btn";
    // 变量定义
    char cash[6];
    char plane[300];
    // 拼接信息
    snprintf(plane, sizeof(plane), "\n持票人姓名: %s  持票人身份证: %s\n", user->name, user->id);
    send(user->connfd, plane, strlen(plane), 0);
    // 发送头
    send_view(user->connfd, utitle);
    // 拼接信息
    // 价格补0
    memset(cash, '\0', sizeof(cash));
    cash_cat(flg_pos->price, cash);
    printf("%s\n", cash);
    snprintf(plane,
             sizeof(plane),
             "*  %s       %s  >>>>>  %s     %s    %s      %s      %s      *\n",
             flg_node->code,
             flg_node->s_plane,
             flg_node->e_plane,
             flg_node->date,
             flg_node->type,
             flg_node->time,
             cash);
    // 发送给客户端
    send(user->connfd, plane, strlen(plane), 0);
    // 发送尾
    send_view(user->connfd, ubtn);
    // 上锁
    pthread_mutex_lock(&m);
    flg_pos_prev->next = flg_pos->next;
    free(flg_pos);
    // 解锁
    pthread_mutex_unlock(&m);
}