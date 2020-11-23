#include "head.h"

int main(int argc,char *argv[])  //  ./jack 192.168.24.xx 50001
{
	//1. 创建一个未连接的套接字
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	//2. 发起连接请求
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;                   //协议
	srvaddr.sin_port = htons(atoi(argv[2]));        //端口号
	inet_pton(AF_INET,argv[1],&srvaddr.sin_addr);   //IP地址

	connect(sockfd,(struct sockaddr *)&srvaddr,len);	

	//3. sockfd是已连接套接字
	//4. 定义一个集合。
	fd_set set;
	int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;
	char buf[100];

	//5. 不断监听这个集合。
	while(1)
	{
		//5.5 每次监听前，记得要全部的文件描述符重新添加到集合中。
		FD_ZERO(&set);
		FD_SET(sockfd,&set);
		FD_SET(STDIN_FILENO,&set);

		//6. 由于最后一个参数填NULL，所以无限等待
		select(max_fd+1,&set,NULL,NULL,NULL);

		//7. 如果有数据到达，则马上返回！
		//究竟谁剩下在集合？ -> FD_ISSET

		if(FD_ISSET(sockfd,&set))  //说明对方想发数据给你
		{
			bzero(buf,sizeof(buf));
			recv(sockfd,buf,sizeof(buf),0);
			printf("from server:%s",buf);

			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
		}

		if(FD_ISSET(STDIN_FILENO,&set))
		{
			bzero(buf,sizeof(buf));
			fgets(buf,sizeof(buf),stdin);
			send(sockfd,buf,strlen(buf),0);

			if(strncmp(buf,"quit",4) == 0)
			{
				break;
			}
		}
	}

	//4. 挂断
	close(sockfd);

	return 0;
}