#include "head.h"

// 全局变量
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
struct manager *boss;
struct plane_manager *p_manager;

void *fun_msg(void *arg);
void *fun_quit(void *arg);

int main(int argc, char const *argv[])
{
    // 创建一个退出线程
    pthread_t tid;
    pthread_create(&tid, NULL, fun_quit, NULL);
    // 初始化套接字
    int sockfd = init_sock(argv[1]);
    // 初始化管理链表的结构体
    boss = malloc(sizeof(struct manager));
    if(boss == NULL)
		printf("malloc boss head error!\n");
    init_boss(boss);
    // 初始化航班链表
    p_manager = malloc(sizeof(struct plane_manager));
    if(p_manager == NULL)
		printf("malloc p_manager head error!\n");
    init_plane(p_manager);
    // 服务器就绪
    printf("server ready!\n\n");

    // 坐等客户端的连接
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int connfd;
    pthread_t t;
    while (1)
    {
        bzero(&cliaddr, len);
        // 每连接一个客户端，都会得到一个已连接套接字
        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
        // 说明有人连接进来
        if (connfd > 0)
        {
            // 先输出是谁连接进来
            printf("new connfd:%d\n", connfd);

            // 分配一个线程，用于监听这个客户端所说的话
            pthread_create(&t, NULL, fun_msg, (void *)&connfd);
        }
    }
}

void *fun_msg(void *arg)
{
    pthread_detach(pthread_self());

    struct timeval v;
    v.tv_sec = 600;
    // v.tv_sec = 5;
    v.tv_usec = 0;
    int connfd = *(int *)arg;
    // 设置超时
    setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &v, sizeof(v));
    login_view_options(connfd);

    pthread_exit(NULL);
}

void *fun_quit(void *arg)
{
    pthread_detach(pthread_self());
    char buf[100];
    while (1)
	{
		bzero(buf, sizeof(buf));
		fgets(buf, sizeof(buf), stdin); //包含'\n'在内

		if (strncmp(buf, "quit", 4) == 0)
		{
            quit_server();
		}
	}
}