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

int main(int argc,char *argv[])
{
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

	//3. 这时候服务器有可能会收到数据，也有可能要发送数据
	//4. 定义一个集合。
	fd_set set;
	int max_fd = connfd > STDIN_FILENO ? connfd : STDIN_FILENO;
	char buf[100];

	//5. 不断监听这个集合。
	while(1)
	{
		//5.5 每次监听前，记得要全部的文件描述符重新添加到集合中。
		FD_ZERO(&set);
		FD_SET(connfd,&set);
		FD_SET(STDIN_FILENO,&set);

		//6. 由于最后一个参数填NULL，所以无限等待
		select(max_fd+1,&set,NULL,NULL,NULL);

		//7. 如果有数据到达，则马上返回！
		//究竟谁剩下在集合？ -> FD_ISSET

		if(FD_ISSET(connfd,&set))  //说明对方想发数据给你
		{
			bzero(buf,sizeof(buf));
			recv(connfd,buf,sizeof(buf),0);
			printf("from client:%s",buf);

			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
		}

		if(FD_ISSET(STDIN_FILENO,&set))
		{
			bzero(buf,sizeof(buf));
			fgets(buf,sizeof(buf),stdin);
			send(connfd,buf,strlen(buf),0);

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