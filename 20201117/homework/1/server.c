#include "head.h"

int sockfd;

void *snd_fun(void *arg)
{
	int connfd = *(int *)arg;

	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);  //包含'\n'在内
		send(connfd,buf,strlen(buf),0);

		if(strncmp(buf,"quit",4) == 0)
		{
			close(connfd);
			close(sockfd);
			exit(0);
		}
	}

}

void *rcv_fun(void *arg)
{
	int connfd = *(int *)arg;

	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		recv(connfd,buf,sizeof(buf),0);
		printf("from client:%s",buf);

		if(strncmp(buf,"quit",4) == 0)
		{
			close(connfd);
			close(sockfd);
			exit(0);
		}
	}
}

int init_sock(char *port)
{
	//1. 创建未连接套接字
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	//2. 绑定IP地址
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

int main(int argc,char *argv[])  //./Rose 50002
{
	//1. 初始化套接字，得到一个监听套接字
	int sockfd;
	sockfd = init_sock(argv[1]);

	//2. 等待对端的连接
	int connfd;
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	bzero(&cliaddr,len);

	connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
	if(connfd > 0)
	{
		printf("new connfd : %d\n",connfd);
		printf("new connection : %s\n",inet_ntoa(cliaddr.sin_addr));
	}

	//3. 创建两个子线程，一个收，一个发
	pthread_t t1,t2;
	pthread_create(&t1,NULL,snd_fun,(void *)&connfd);
	pthread_create(&t2,NULL,rcv_fun,(void *)&connfd);

	//4. 接合线程
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}