#include "head.h"

int init_sock(char const *port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in srvadr;
    socklen_t len = sizeof(srvadr);
    bzero(&srvadr, len);

    srvadr.sin_family = AF_INET;
    srvadr.sin_port = htons(atoi(port));
    srvadr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&srvadr, len);

    listen(sockfd, 5);

    return sockfd;
}

void recv_fun(const char *tip, int connfd, char *buf)
{
    int ret;
    // 用户输入
    while (1)
    {
        bzero(buf, sizeof(buf));
        // 发送提示
        send(connfd, tip, strlen(tip), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        else
        {
            break;
        }
    }
}