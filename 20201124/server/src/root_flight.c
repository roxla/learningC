#include "head.h"

void flight_del(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    // 判断是否为空链
    if (list_empty(&p_manager->head->list))
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "当前没有任何航班!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 用户输入
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入要删除的航班号:\n");
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
    // 判断航班是否存在
    pt_flg flg_node, flg_n;
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            break;
        }
    }
    struct list_head *pos = &flg_node->list;
    if (pos == &p_manager->head->list)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "该航班不存在!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 上锁
    char flg_name[10];
    pthread_mutex_lock(&m);
    // 修改指向
    list_del(pos);
    // 释放堆空间
    memset(flg_name, '\0', sizeof(flg_name));
    strncpy(flg_name, flg_node->code, 5);
    free(flg_node);
    // 修改航班数
    p_manager->count--;
    // 解锁
    pthread_mutex_unlock(&m);
    // 将航班信息存入文件中
    add_flight_file(p_manager->head);
    // 发送提示
    snprintf(tip_cli, sizeof(tip_cli), "删除成功!\n");
    send(user->connfd, tip_cli, strlen(tip_cli), 0);

    // 给所有拥有该航班的在线用户发送提示
    snprintf(tip_cli, sizeof(tip_cli), "航班%s已下线，请前去改签或退票!\n", flg_name);
    pt_uinfo get_node, n;
    pt_myflg flg_pos;
    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {
        for (flg_pos = get_node->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
        {
            if (!strncmp(flg_pos->flight, flg_name, 5))
            {
                if (get_node->connfd != -1)
                {
                    send(get_node->connfd, tip_cli, strlen(tip_cli), 0);
                }
            }
        }
    }
}

void flight_add(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    if (p_manager->count >= MAX_PLANE)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "航班数量过多!\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    snprintf(tip_cli, sizeof(tip_cli), "请输入要添加的航班号:\n");
    // 获取航班号
    recv_fun(tip_cli, user->connfd, buf);
    if ((strlen(buf) - 1) != 5)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "输入的航班号格式不正确\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        return;
    }
    // 判断航班是否存在
    pt_flg flg_node, flg_n;
    list_for_each_entry_safe(flg_node, flg_n, &p_manager->head->list, list)
    {
        if (!strncmp(flg_node->code, buf, strlen(buf) - 1))
        {
            break;
        }
    }
    struct list_head *pos = &flg_node->list;
    if (pos == &p_manager->head->list)
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "\n添加新航班!\n\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        add_new_plane(user->connfd, buf);
    }
    else
    {
        // 发送提示
        snprintf(tip_cli, sizeof(tip_cli), "\n修改现有航班!\n\n");
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 上锁
        pthread_mutex_lock(&m);
        change_old_plane(user->connfd, flg_node);
        // 解锁
        pthread_mutex_unlock(&m);
    }
}

void add_new_plane(int connfd, char *code)
{
    int temp;
    char tips[] = "error\n";
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    char plane[50] = {0};
    char s_plane[3] = {0};
    char e_plane[3] = {0};
    // 创建新节点
    // 申请堆空间给新节点
    pt_flg pos = malloc(sizeof(st_flg));
    if (pos == NULL)
    {
        perror("new_node malloc failed");
        return;
    }
    bzero(pos, sizeof(st_flg));
    // 将数据给入新节点的数据域
    // 航班号 唯一
    memset(plane, '\0', sizeof(plane));
    strncpy(plane, code, 5);
    memcpy(pos->code, plane, strlen(plane));
    // 余票
    snprintf(tip_cli, sizeof(tip_cli), "请输入余票:\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp > 0)
    {
        pos->num = temp;
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 金额
    snprintf(tip_cli, sizeof(tip_cli), "请输入金额:\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp > 0)
    {
        pos->cash = temp;
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 飞机类型
    snprintf(tip_cli, sizeof(tip_cli), "请输入飞机类型:\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    if ((strlen(buf) - 1) == 4)
    {
        strncpy(plane, buf, 4);
        memcpy(pos->type, plane, strlen(plane));
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 起飞时间
    snprintf(tip_cli, sizeof(tip_cli), "请输入起飞时间:\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    if ((strlen(buf) - 1) == 5)
    {
        strncpy(plane, buf, 5);
        char all_time[9];
        snprintf(tip_cli, sizeof(tip_cli), "请选择时间(1.上午 2.下午):\n");
        recv_fun(tip_cli, connfd, buf);
        temp = atoi(buf);
        if (temp == 1)
        {
            snprintf(all_time, sizeof(all_time), "%s AM", plane);
        }
        else
        {
            snprintf(all_time, sizeof(all_time), "%s PM", plane);
        }
        memcpy(pos->time, all_time, strlen(all_time));
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 班期
    char date[13];
    snprintf(tip_cli, sizeof(tip_cli), "请输入班期(年):\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp % 1000 > 0)
    {
        int year = temp;
        snprintf(tip_cli, sizeof(tip_cli), "请输入班期(月):\n");
        recv_fun(tip_cli, connfd, buf);
        temp = atoi(buf);
        if (temp > 0 && temp < 13)
        {
            int mon = temp;
            snprintf(tip_cli, sizeof(tip_cli), "请输入班期(日):\n");
            recv_fun(tip_cli, connfd, buf);
            temp = atoi(buf);
            if (temp > 0 && (((mon % 2) == 0 && temp <= 30) || ((mon % 2) == 1 && temp <= 31)))
            {
                int day = temp;

                int f_day = 0;
                int f_mon = 0;

                if (mon < 10)
                {
                    f_mon = 1;
                }
                if (day < 10)
                {
                    f_day = 1;
                }
                if (f_mon && f_day)
                {
                    snprintf(date, sizeof(date), "%d-0%d-0%d", year, mon, day);
                }
                else if (f_mon)
                {
                    snprintf(date, sizeof(date), "%d-0%d-%d", year, mon, day);
                }
                else if (f_day)
                {
                    snprintf(date, sizeof(date), "%d-%d-0%d", year, mon, day);
                }
                else
                {
                    snprintf(date, sizeof(date), "%d-%d-%d", year, mon, day);
                }
                memcpy(pos->date, date, strlen(date));
            }
            else
            {
                snprintf(tip_cli, sizeof(tip_cli), "日输入不正确!\n");
                send(connfd, tip_cli, strlen(tip_cli), 0);
                free(pos);
                return;
            }
        }
        else
        {
            snprintf(tip_cli, sizeof(tip_cli), "月输入不正确!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            free(pos);
            return;
        }
    }
    else
    {
        snprintf(tip_cli, sizeof(tip_cli), "年输入不正确!\n");
        send(connfd, tip_cli, strlen(tip_cli), 0);
        free(pos);
        return;
    }
    // 起点站
    snprintf(tip_cli, sizeof(tip_cli), "请输入起点站:\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    if ((strlen(buf) - 1) == 2)
    {
        strncpy(s_plane, buf, 2);
        memcpy(pos->s_plane, s_plane, strlen(s_plane));
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 终点站
    snprintf(tip_cli, sizeof(tip_cli), "请输入终点站:\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    if ((strlen(buf) - 1) == 2)
    {
        strncpy(e_plane, buf, 2);
        if (!strcmp(e_plane, s_plane))
        {
            snprintf(tip_cli, sizeof(tip_cli), "终点站和起点站不能相同!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            free(pos);
            return;
        }
        else
        {
            memcpy(pos->e_plane, e_plane, strlen(e_plane));
        }
    }
    else
    {
        send(connfd, tips, strlen(tips), 0);
        free(pos);
        return;
    }
    // 上锁
    pthread_mutex_lock(&m);
    // 修改指向
    list_add_tail(&pos->list, &p_manager->head->list);
    // 解锁
    pthread_mutex_unlock(&m);
    // 将航班信息存入文件中
    add_flight_file(p_manager->head);
    // 修改完成
    snprintf(tip_cli, sizeof(tip_cli), "添加成功！\n");
    send(connfd, tip_cli, strlen(tip_cli), 0);

    // plane_info_list_show(p_manager->head);
}

void change_old_plane(int connfd, pt_flg pos)
{
    int temp;
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    char plane[50];
    // 余票
    snprintf(tip_cli, sizeof(tip_cli), "请输入余票(输入-1则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp >= 0)
    {
        pos->num = temp;
    }
    // 金额
    snprintf(tip_cli, sizeof(tip_cli), "请输入金额(输入0则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp > 0)
    {
        pos->cash = temp;
    }
    // 飞机类型
    snprintf(tip_cli, sizeof(tip_cli), "请输入飞机类型(输入-1则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    temp = atoi(buf);
    if (temp != -1)
    {
        strncpy(plane, buf, 4);
        printf("type: %s\n", plane);
        snprintf(pos->type, sizeof(pos->type), "%s", plane);
    }
    // 起飞时间
    snprintf(tip_cli, sizeof(tip_cli), "请输入起飞时间(输入-1则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    temp = atoi(buf);
    if (temp != -1)
    {
        printf("time1: %s\n", plane);
        strncpy(plane, buf, 5);
        printf("time2: %s\n", plane);
        char all_time[9];
        snprintf(tip_cli, sizeof(tip_cli), "请选择时间(1.上午 2.下午):\n");
        recv_fun(tip_cli, connfd, buf);
        temp = atoi(buf);
        if (temp == 1)
        {
            snprintf(all_time, sizeof(all_time), "%s AM", plane);
        }
        else
        {
            snprintf(all_time, sizeof(all_time), "%s PM", plane);
        }
        memcpy(pos->time, all_time, strlen(all_time));
    }
    // 班期
    snprintf(tip_cli, sizeof(tip_cli), "是否修改班期(1.修改 0.默认):\n");
    recv_fun(tip_cli, connfd, buf);
    temp = atoi(buf);
    if (temp)
    {
        char date[13];

        snprintf(tip_cli, sizeof(tip_cli), "请输入年:\n");
        recv_fun(tip_cli, connfd, buf);
        temp = atoi(buf);
        if (temp % 1000 > 0)
        {
            int year = temp;
            snprintf(tip_cli, sizeof(tip_cli), "请输入月:\n");
            recv_fun(tip_cli, connfd, buf);
            temp = atoi(buf);
            if (temp > 0 && temp < 13)
            {
                int mon = temp;
                snprintf(tip_cli, sizeof(tip_cli), "请输入日:\n");
                recv_fun(tip_cli, connfd, buf);
                temp = atoi(buf);
                if (temp > 0 && (((mon % 2) == 0 && temp <= 30) || ((mon % 2) == 1 && temp <= 31)))
                {
                    int day = temp;

                    int f_day = 0;
                    int f_mon = 0;

                    if (mon < 10)
                    {
                        f_mon = 1;
                    }
                    if (day < 10)
                    {
                        f_day = 1;
                    }
                    if (f_mon && f_day)
                    {
                        snprintf(date, sizeof(date), "%d-0%d-0%d", year, mon, day);
                    }
                    else if (f_mon)
                    {
                        snprintf(date, sizeof(date), "%d-0%d-%d", year, mon, day);
                    }
                    else if (f_day)
                    {
                        snprintf(date, sizeof(date), "%d-%d-0%d", year, mon, day);
                    }
                    else
                    {
                        snprintf(date, sizeof(date), "%d-%d-%d", year, mon, day);
                    }
                    memcpy(pos->date, date, strlen(date));
                }
                else
                {
                    snprintf(tip_cli, sizeof(tip_cli), "日输入不正确!\n");
                    send(connfd, tip_cli, strlen(tip_cli), 0);
                }
            }
            else
            {
                snprintf(tip_cli, sizeof(tip_cli), "月输入不正确!\n");
                send(connfd, tip_cli, strlen(tip_cli), 0);
            }
        }
        else
        {
            snprintf(tip_cli, sizeof(tip_cli), "年输入不正确!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
    // 起点站
    snprintf(tip_cli, sizeof(tip_cli), "请输入起点站(输入-1则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    temp = atoi(buf);
    if (temp != -1)
    {
        strncpy(plane, buf, 2);
        if (strcmp(plane, pos->e_plane))
        {
            memcpy(pos->s_plane, plane, strlen(plane));
        }
        else
        {
            snprintf(tip_cli, sizeof(tip_cli), "起点站与终点站不能相同!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
    // 终点站
    snprintf(tip_cli, sizeof(tip_cli), "请输入终点站(输入-1则为默认):\n");
    recv_fun(tip_cli, connfd, buf);
    memset(plane, '\0', sizeof(plane));
    temp = atoi(buf);
    if (temp != -1)
    {
        strncpy(plane, buf, 2);
        if (strcmp(plane, pos->s_plane))
        {
            memcpy(pos->e_plane, plane, strlen(plane));
        }
        else
        {
            snprintf(tip_cli, sizeof(tip_cli), "终点站与起点站不能相同!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
    // 将航班信息存入文件中
    add_flight_file(p_manager->head);
    // 修改完成
    snprintf(tip_cli, sizeof(tip_cli), "修改成功！\n");
    send(connfd, tip_cli, strlen(tip_cli), 0);
}