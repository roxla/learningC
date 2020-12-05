#include "head.h"

void *snd_fun(void *arg)
{
    int sockfd = *(int *)arg;

    char buf[60];
    while (1)
    {
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin); //包含'\n'在内
        send(sockfd, buf, strlen(buf), 0);

        // if (strncmp(buf, "quit", 4) == 0)
        // {
        //     close(sockfd);
        //     exit(0);
        // }
    }
}

void *rcv_fun(void *arg)
{
    int sockfd = *(int *)arg;

    char buf[60];
    while (1)
    {
        bzero(buf, sizeof(buf));
        recv(sockfd, buf, sizeof(buf), 0);
        printf("%s", buf);

        if (strncmp(buf, "quit", 4) == 0)
        {
            close(sockfd);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) //  ./client 192.168.24.4 50002
{
    //1. 创建未连接套接字
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //2. 直接发起连接
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    bzero(&cliaddr, len);

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &cliaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&cliaddr, len);

    //3. 创建两个线程，用于收和发
    pthread_t t1, t2;
    pthread_create(&t1, NULL, snd_fun, (void *)&sockfd);
    pthread_create(&t2, NULL, rcv_fun, (void *)&sockfd);

    //4. 接合线程
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}