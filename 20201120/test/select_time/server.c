
#include "head.h"

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

void *fun(void *arg)
{
	int i;
	for(i=0;i<1000;i++)
	{
		printf("%d\n",i);
		sleep(1);
	}
}

int main(int argc,char *argv[])
{
	//0. 开启一个计算时间的线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun,NULL);

	//1. 初始化套接字
	int sockfd;
	sockfd = init_sock(argv[1]);

	//2. 坐等客户端的连接
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	bzero(&cliaddr,len);
	int connfd;
	connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
	if(connfd > 0)
	{
		printf("new usr connect!\n");
	}

	//3. 使用select函数去监听这个套接字，然后如果有数据到达就读取，如果没有则超时。
	fd_set set;
	struct timeval v;
	int ret;
	char buf[100];
	while(1)
	{
		FD_ZERO(&set);
		FD_SET(connfd,&set);

		v.tv_sec = 5;
		v.tv_usec = 0;

		ret = select(connfd+1,&set,NULL,NULL,&v);  //这个select函数只会在5s内阻塞等待。
		if(ret == -1)
		{
			printf("select error!\n");
		}

		if(ret == 0)
		{
			printf("timeout!\n");
		}

		if(ret > 0)
		{
			//代表有数据到达
			bzero(buf,sizeof(buf));
			recv(connfd,buf,sizeof(buf),0);
			printf("from client:%s",buf);

			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
		}

	}
	close(connfd);
	close(sockfd);
	return 0;
}