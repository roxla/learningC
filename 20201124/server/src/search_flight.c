#include "head.h"

// 查询航班
void search_fligth_list(pt_uinfo user)
{
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/flight";
    int num;
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    // 功能选项
    while (1)
    {
        // 发送界面
        while (send_view(user->connfd, filename))
            ;
        num = num_msg(user->connfd);
        char temp[7];
        char date[13];
        bzero(date, 13);
        switch (num)
        {
        case 1: // 目的地
            while (1)
            {
                bzero(buf, sizeof(buf));
                snprintf(tip_cli, sizeof(tip_cli), "请输入目的地:\n");
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
                    break;
            }
            view_flight_search(user, NULL, buf, NULL, NULL, 0);
            break;
        case 2: // 班期
            while (1)
            {
                bzero(buf, sizeof(buf));
                snprintf(tip_cli, sizeof(tip_cli), "请输入班期(例如: 2010-01-11):\n");
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
                    break;
            }
            view_flight_search(user, NULL, NULL, buf, NULL, 0);
            break;
        case 3: // 机型
            while (1)
            {
                bzero(buf, sizeof(buf));
                snprintf(tip_cli, sizeof(tip_cli), "请输入机型:\n");
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
                    break;
            }
            view_flight_search(user, NULL, NULL, NULL, buf, 0);
            break;
        case 4: // 价格
            while (1)
            {
                bzero(buf, sizeof(buf));
                snprintf(tip_cli, sizeof(tip_cli), "请输入价格:\n");
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
                    break;
            }
            view_flight_search(user, NULL, NULL, NULL, NULL, atoi(buf));
            break;
        default: // 退出
            return;
            break;
        }
    }
}

// 快速查询航班
void quick_search(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret;
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入要查询的航班号:\n");
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
    view_flight_search(user, buf, NULL, NULL, NULL, 0);
}