
#include "head.h"

int main(int argc,char *argv[])  //  ./Rose 50002
{
	//1. 创建UDP套接字
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	//2. 绑定IP地址，端口号，协议到UDP协议中
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(argv[1]));
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr *)&srvaddr,len);

	//3. 坐等对方写信给我
	char buf[100];
	struct sockaddr_in cliaddr;
	

	while(1)
	{
		bzero(&cliaddr,len);
		bzero(buf,sizeof(buf));

		recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&cliaddr,&len); 
		
		//4. 输出信的内容
		printf("%s:%s",inet_ntoa(cliaddr.sin_addr),buf);

		if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
	}
	
	//5. 回收套接字资源
	close(sockfd);

	return 0;
}