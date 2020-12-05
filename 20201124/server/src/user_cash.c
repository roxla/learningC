#include "head.h"

// 查询余额
void search_cash(pt_uinfo user)
{
    char buf[MAX_IO];
    snprintf(buf, sizeof(buf), "\n=== 当前余额: %d元 ===\n\n", user->cash);
    send(user->connfd, buf, strlen(buf), 0);

    return;
}

// 余额充值
void charge_cash(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    char tip_many[] = "To many!\n";
    int ret, temp;
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/cash";
    if (user->cash >= 1000000000)
    {
        send(user->connfd, tip_many, strlen(tip_many), 0);
        return;
    }
    send_view(user->connfd, filename);
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入充值金额:\n");
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        if ((strlen(buf) - 1) >= 9)
        {
            send(user->connfd, tip_many, strlen(tip_many), 0);
            return;
        }
        else
        {
            break;
        }
    }
    temp = atoi(buf);
    printf("cash: %d", temp);

    //访问链表前要上锁
    pthread_mutex_lock(&m);

    user->cash += temp;
    if (user->vip == 0)
    {
        if (temp >= 1000)
        {
            user->vip++;
            snprintf(tip_cli, sizeof(tip_cli), "恭喜你成为 VIP1!\n");
            // 发送提示
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
    }
    else if (user->vip < 10)
    {
        if (temp >= 2000)
        {
            user->vip++;
            snprintf(tip_cli, sizeof(tip_cli), "恭喜你升级为 VIP%d!\n", user->vip);
            // 发送提示
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
    }

    //访问链表后要解锁
    pthread_mutex_unlock(&m);

    snprintf(tip_cli, sizeof(tip_cli), "\n=== 已成功充值%d元 ===\n", temp);
    send(user->connfd, tip_cli, strlen(tip_cli), 0);
    snprintf(tip_cli, sizeof(tip_cli), "\n=== 当前余额%d元 ===\n\n", user->cash);
    send(user->connfd, tip_cli, strlen(tip_cli), 0);

    return;
}