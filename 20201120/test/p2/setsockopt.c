
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
	for(i = 0; i < 1000; ++i)
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

	//2. 阻塞等待客户端连接
	int connfd;
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	bzero(&cliaddr,len);

	connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
	if(connfd > 0)
	{
		printf("new connfd:%d\n",connfd);
		printf("new connection:%s\n",inet_ntoa(cliaddr.sin_addr));
	}

	//3. 设置超时属性
	struct timeval v;
	v.tv_sec = 5;
	v.tv_usec = 0;
	setsockopt(connfd,SOL_SOCKET,SO_RCVTIMEO,&v,sizeof(v));

	int ret;
	//4. 读取客户端的数据
	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		ret = recv(connfd,buf,sizeof(buf),0);

		//说明有数据到达
		if(ret > 0)
		{
			printf("from client:%s",buf);
		}

		if(ret == -1)
		{
			printf("timout!\n");
		}

		if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
	}

	//5. 关闭套接字
	close(sockfd);
	close(connfd);

	return 0;
}