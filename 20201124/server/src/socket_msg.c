#include "head.h"

// 获取客户端选项
int num_msg(int connfd)
{
    char buf[40];
    char tips[] = "请输入你的选项(例如1):\n";
    int ret, num;
    while (1)
    {
        bzero(buf, sizeof(buf));
        send(connfd, tips, strlen(tips), 0);
        ret = recv(connfd, buf, sizeof(buf), 0);
        num = atoi(buf);
        if (ret == -1)
        {
            return -1;
        }
        if (num == 0)
            send(connfd, "error\n", 6, 0);
        else
            break;
    }

    return num;
}

// 发送用户数据
void send_user(int connfd)
{
    
}