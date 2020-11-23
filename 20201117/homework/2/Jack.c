#include "head.h"

/*
客户端： 负责发送数据给服务器
*/

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

	//3. 不断发送数据给服务器
	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		//从键盘中获取数据，并存放到buf中。
		fgets(buf,sizeof(buf),stdin);
		//将buf发送给服务器
		send(sockfd,buf,strlen(buf),0);

		//要接收服务器回发的数据
		bzero(buf,sizeof(buf));
		recv(sockfd,buf,sizeof(buf),0);
		printf("from server:%s",buf);

		if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
	}

	//4. 挂断
	close(sockfd);

	return 0;
}