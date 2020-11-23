#include "head.h"
#include "kernel_list.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
struct list_node *head = NULL;

//链表的节点
struct list_node{
	int connfd;  //数据域
	struct list_head list;  //指针域
};

int init_sock(char *port)
{
	//1. 创建TCP套接字
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	//2. 绑定地址
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(port));
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr *)&srvaddr,len);

	//3. 设置监听套接字
	listen(sockfd,5);

	return sockfd;
}

struct list_node *init_list_head(struct list_node *head)  //head = NULL
{
	//1. 为头节点申请空间
	head = (struct list_node *)malloc(sizeof(struct list_node));
	if(head == NULL)
		printf("malloc error!\n");

	//2. 为头节点的数据域与指针域赋值
	//数据域无效
	//指针域有效
	INIT_LIST_HEAD(&(head->list));

	return head;
}

int msg_broadcast(const char *buf,struct list_node *peer)
{
	//buf: 就是peer所群发的内容
	//peer: 就是发送者
	//群发特点： 除了自己收不到，其他在线的都可以收到

	struct list_node *p = NULL;

	//访问之前,要上锁
	pthread_mutex_lock(&m);

	list_for_each_entry(p,&(head->list),list)
	{
		if(p->connfd == peer->connfd)
		{
			continue;
		}

		send(p->connfd,buf,strlen(buf),0);
	}

	pthread_mutex_unlock(&m);

	return 0;
}

int msg_send(int recv_id,const char *buf)
{
	//recv_id: 接收方的id号
	//buf: 私聊的内容
	//私聊特点： 遍历这个链表，要找打id号与recv_id一致的才发送。

	pthread_mutex_lock(&m);

	struct list_node *p = NULL;
	list_for_each_entry(p,&(head->list),list)
	{
		if(p->connfd == recv_id)  //找到目标
		{
			send(p->connfd,buf,strlen(buf),0);
			pthread_mutex_unlock(&m);
			return 0;
		}
	}

	printf("Not found this usr!\n");
	pthread_mutex_unlock(&m);

	return -1;
}

void *fun(void *arg)
{
	pthread_detach(pthread_self());
	//这个线程任务：就是不断监听客户端所说的话即可。
	struct list_node *peer = (struct list_node *)arg;

	//服务员不断读取这个客户端的数据就行了
	char buf[100];
	char *tmp = NULL;
	int recv_id;

	while(1)
	{
		bzero(buf,sizeof(buf));
		recv(peer->connfd,buf,sizeof(buf),0);
		//printf("from peer:%s",buf);

		if(strncmp(buf,"quit",4) == 0)
		{
			//客户端想退出
			close(peer->connfd);

			pthread_mutex_lock(&m);

			list_del(&(peer->list));

			pthread_mutex_unlock(&m);

			free(peer);
			break;  //线程也不需要继续监听了
		}

		tmp = strstr(buf,":");
		if(tmp == NULL)  //说明没有找到: 说明是群发  hello
		{
			msg_broadcast(buf,peer);
		}
		else{  //找到了: 说明是私聊   5:hello
			recv_id = atoi(buf);  //5
			msg_send(recv_id,tmp+1);
		}
	}
	pthread_exit(NULL);
}

int main(int argc,char *argv[])  // ./server 50001
{
	//0. 初始化服务器中存放客户端的connfd的链表
	head = init_list_head(head);

	//1. 初始化套接字
	int sockfd;
	sockfd = init_sock(argv[1]);

	//2. 服务器不断等待客户端的连接
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	int connfd;
	struct list_node *new = NULL;
	pthread_t tid;
	while(1)
	{
		bzero(&cliaddr,len);

		//3. 每连接一个客户端，都会得到一个已连接套接字
		connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);

		//4. 说明有人连接进来
		if(connfd > 0)
		{
			//5. 先输出是谁连接进来
			printf("new connfd:%d\n",connfd);
			printf("new connection:%s\n",inet_ntoa(cliaddr.sin_addr));

			//6. 就把这个客户端的信息(connfd)存放到链表中
			new = (struct list_node *)malloc(sizeof(struct list_node));
			if(new != NULL)  //申请空间成功
			{
				//7. 为新节点的数据域与指针域赋值
				new->connfd = connfd;   //数据域

				//访问链表前要上锁
				pthread_mutex_lock(&m);

				list_add_tail(&(new->list),&(head->list)); //指针域与尾插

				//访问链表后要解锁
				pthread_mutex_unlock(&m);

				//8. 只要尾插成功，都给你分配一个线程，用于监听这个客户端所说的话。
				pthread_create(&tid,NULL,fun,(void *)new);
			}
		}
		//9. 继续accept()等待客户端连接。
	}
}