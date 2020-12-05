#include "head.h"

typedef struct usr_list
{
	// 数据域
	int clifd;

	// 指针域
	struct usr_list *next; //下节点地址
	struct usr_list *prev; //上节点地址
} usr_st, *usr_pt;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int init_sock(const char *port);
usr_pt init_cli(void);
int add_cli_tail(int new_data, usr_pt head);
int del_cli(int del_data, usr_pt head);

int main(int argc, char const *argv[])
{
	// 初始化套接字
	int sockfd = init_sock(argv[1]);
	// 初始化客户端结构体
	usr_pt user = init_cli();

	// 设置监听套接字为非阻塞属性
	int state;
	state = fcntl(sockfd, F_GETFL);
	state |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, state);

	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	int connfd, ret;
	char buf[100];
	usr_pt pos = NULL;

	while (1)
	{
		// 不断非阻塞地等待客户端的连接
		bzero(&cliaddr, len);
		connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);

		// 如果真的有人连接，则将这个人的套接字塞到链表中
		if (connfd > 0)
		{
			// 先将这个套接字设置为非阻塞
			state = fcntl(connfd, F_GETFL);
			state |= O_NONBLOCK;
			fcntl(connfd, F_SETFL, state);

			// 将这个connfd存储到链表中
			// 访问之前,要上锁
			pthread_mutex_lock(&m);
			ret = add_cli_tail(connfd, user);
			if (ret == 1)
				close(connfd);
			pthread_mutex_unlock(&m);

			// 输出一下是谁连接进来
			printf("new connfd:%d\n", connfd);
			printf("new connection:%s\n", inet_ntoa(cliaddr.sin_addr));
		}

		// 如果没有人连接，则开始询问所有连接服务器上的客户端有没有话说
		for (pos = user->next; pos != user; pos = pos->next)
		{
			bzero(buf, sizeof(buf));
			if (recv(pos->clifd, buf, sizeof(buf), 0) >= 0)
			{
				printf("from %d client : %s", pos->clifd, buf);
				if (strncmp(buf, "quit", 4) == 0)
				{
					close(pos->clifd);

					pthread_mutex_lock(&m);

					del_cli(pos->clifd, user);

					pthread_mutex_unlock(&m);
				}
			}
			else
				continue;
		}
	}

	return 0;
}

// 初始化套接字
int init_sock(const char *port)
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

usr_pt init_cli(void)
{
	// 申请一个堆空间给头节点
	usr_pt head = malloc(sizeof(usr_st));
	if (head == NULL)
	{
		perror("head malloc failed");
		return NULL;
	}

	// 修改指针域，使他们都指向本身
	head->prev = head;
	head->next = head;

	// 将堆空间返回
	return head;
}

int add_cli_tail(int new_data, usr_pt head)
{
	// 申请堆空间给新节点
	usr_pt new_node = malloc(sizeof(usr_st));
	if (new_node == NULL)
	{
		perror("new_node malloc failed");
		return 1;
	}

	// 将数据给入新节点的数据域
	new_node->clifd = new_data;

	// 修改指向
	// 操作新节点，prev指前节点(尾)，next指向后节点(头)
	new_node->prev = head->prev;
	new_node->next = head;
	// 操作前后节点，前节点(尾)next，和后节点(头)prev都指向新节点
	head->prev->next = new_node;
	head->prev = new_node;

	return 0;
}

int del_cli(int del_data, usr_pt head)
{
	if (head->next == head)
	{
		printf("Empty");
		return 0;
	}
	usr_pt pos = NULL;
	for (pos = head->next; pos != head; pos = pos->next)
	{
		if (pos->clifd == del_data)
			break;
	}
	if (pos == head)
		return 0;

	pos->next->prev = pos->prev;
	pos->prev->next = pos->next;
	free(pos);

	return 1;
}