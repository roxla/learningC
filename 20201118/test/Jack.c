#include "head.h"

int main(int argc,char *argv[]) // ./Jack 192.168.24.3 50002
{
	//1. 创建UDP套接字
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	//2. 直接写信
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&srvaddr.sin_addr);

	char buf[100];
	while(1)
	{
		bzero(buf,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);
		sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&srvaddr,len);

		if(strncmp(buf,"quit",4) == 0)
		{
			break;
		}
	}
	
	//3. 回收资源
	close(sockfd);
	return 0;
}