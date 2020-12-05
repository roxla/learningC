#include "head.h"

// 静态全局变量
// 结构头
static char title[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/flight_struct/flight_title";
// 结构尾
static char btn[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/flight_struct/flight_btn";
// 结构分割线
static char line[] = "------------------------------------------------------------------------------------\n";

// 显示全部航班
void view_flight_list(pt_uinfo user)
{
    // 变量定义
    char plane[300];
    char cash[6];
    pt_flg get_node, n;
    // 发送头
    send_view(user->connfd, title);
    list_for_each_entry_safe(get_node, n, &p_manager->head->list, list)
    {
        // 发送分割线
        send(user->connfd, line, strlen(line), 0);
        // 价格补0
        cash_cat(get_node->cash, cash);
        // 拼接信息
        snprintf(plane,
                 sizeof(plane),
                 "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                 get_node->code,
                 get_node->s_plane,
                 get_node->e_plane,
                 get_node->date,
                 get_node->type,
                 get_node->time,
                 cash,
                 get_node->num);
        // 发送给客户端
        send(user->connfd, plane, strlen(plane), 0);
    }
    // 发送分割线
    send(user->connfd, line, strlen(line), 0);
    // 发送尾
    send_view(user->connfd, btn);
}

// 显示查询航班
int view_flight_search(pt_uinfo user, char *code, char *e_plane, char *date, char *p_type, int price)
{
    // 变量定义
    char plane[300];
    char cash[6];
    pt_flg get_node, n;
    // 发送头
    send_view(user->connfd, title);
    list_for_each_entry_safe(get_node, n, &p_manager->head->list, list)
    {
        if (code)
        {
            if (!strncmp(get_node->code, code, strlen(code) - 1))
            {
                // 发送分割线
                send(user->connfd, line, strlen(line), 0);
                // 价格补0
                printf("%d\n", get_node->cash);
                cash_cat(get_node->cash, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash,
                         get_node->num);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
        else if (e_plane)
        {
            if (!strncmp(get_node->e_plane, e_plane, strlen(e_plane) - 1))
            {
                // 发送分割线
                send(user->connfd, line, strlen(line), 0);
                // 价格补0
                cash_cat(get_node->cash, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash,
                         get_node->num);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
        else if (date)
        {
            if (!strncmp(get_node->date, date, strlen(date) - 1))
            {
                // 发送分割线
                send(user->connfd, line, strlen(line), 0);
                // 价格补0
                cash_cat(get_node->cash, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash,
                         get_node->num);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
        else if (p_type)
        {
            if (!strncmp(get_node->type, p_type, strlen(p_type) - 1))
            {
                // 发送分割线
                send(user->connfd, line, strlen(line), 0);
                // 价格补0
                cash_cat(get_node->cash, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash,
                         get_node->num);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
        else if (price)
        {
            if (get_node->cash == price)
            {
                // 发送分割线
                send(user->connfd, line, strlen(line), 0);
                // 价格补0
                cash_cat(get_node->cash, cash);
                // 拼接信息
                snprintf(plane,
                         sizeof(plane),
                         "%s       %s  >>>>>  %s     %s    %s      %s      %s     %d\n",
                         get_node->code,
                         get_node->s_plane,
                         get_node->e_plane,
                         get_node->date,
                         get_node->type,
                         get_node->time,
                         cash,
                         get_node->num);
                // 发送给客户端
                send(user->connfd, plane, strlen(plane), 0);
            }
        }
    }
    // 发送分割线
    send(user->connfd, line, strlen(line), 0);
    // 发送尾
    send_view(user->connfd, btn);

    return 0;
}

// 价格补0 (通过参数获取值)
void cash_cat(int num, char *cost)
{
    char cash[5];
    char temp[4];
    int i;
    snprintf(cash, sizeof(cash), "%d", num);
    bzero(temp, 4);
    for (i = 0; i < 4 - strlen(cash); i++)
        strncat(temp, "0", 1);
    snprintf(cash, sizeof(cash), "%s%d", temp, num);
    memcpy(cost, cash, strlen(cash)); // 赋值cash给cost
}
