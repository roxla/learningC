#include "head.h"

/*
服务器端： 负责读取客户端发送过来的数据
*/

int main(int argc,char *argv[])  // ./Rose 50001
{
	//1. 创建一个未连接的套接字
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	//2. 绑定IP地址、端口号、协议到未连接套接字上。
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;                 //协议
	srvaddr.sin_port = htons(atoi(argv[1]));      //端口号
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址   

	bind(sockfd,(struct sockaddr *)&srvaddr,len);

	//3. 设置监听套接字
	listen(sockfd,5);

	//4. 等待客户端的连接请求
	int connfd;
	struct sockaddr_in cliaddr;
	bzero(&cliaddr,len);

	connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
	if(connfd > 0)
	{
		printf("new connfd:%d\n",connfd);
		printf("new connection: %s\n",inet_ntoa(cliaddr.sin_addr));
	}

	//5. 不断接收数据
	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		recv(connfd,buf,sizeof(buf),0);
		printf("from jack:%s",buf);

		if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
	}

	//6. 挂断
	close(sockfd);
	close(connfd);

	return 0;
}