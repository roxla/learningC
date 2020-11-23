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

	//1. sockfd  -> 已经准备好了
	//2. 给sockfd添加一个非阻塞属性
	int state;
	state = fcntl(sockfd,F_GETFL);  //state就是文件描述符原有的属性。
	state |= O_NONBLOCK;            //state就是在原有的基础上添加了非阻塞属性。
									//state = state | O_NONBLOCK
	fcntl(sockfd,F_SETFL,state);    //把最后的新属性添加到文件描述符中。

	//3. 再去使用accept就是非阻塞的。
	while(1)
	{
		//非阻塞等待连接
		connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);

		//无论有没有人连接进来，都要输出结果
		//printf("connfd = %d\n",connfd);

		//如果真的有人连接进来，connfd>0  那么就输出名字和退出循环
		if(connfd > 0)
		{
			printf("new connfd:%d\n",connfd);
			printf("new connection: %s\n",inet_ntoa(cliaddr.sin_addr));
			break;
		}

		//如果没有人连接，则继续询问有没有人连接
	}
	
	//5. 不断接收数据

	//1. 准备好connfd。
	//2. 给connfd添加非阻塞属性
	state = fcntl(connfd,F_GETFL);
	state |= O_NONBLOCK;
	fcntl(connfd,F_SETFL,state);  //connfd就是非阻塞的！

	char buf[100];
	int ret;
	while(1)
	{
		bzero(buf,sizeof(buf));
		ret = recv(connfd,buf,sizeof(buf),0);
		//printf("ret = %d\n",ret);
		if(ret >= 0)
		{
			printf("from jack:%s",buf);
		}

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