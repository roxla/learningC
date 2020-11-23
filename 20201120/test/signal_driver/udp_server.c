
//当客户端给我发送数据时，说明sockfd收到了数据并产生信号。
#include "head.h"

int sockfd;
char buf[50];
struct sockaddr_in cliaddr;
socklen_t len = sizeof(cliaddr);

void my_fun(int sig)
{
	bzero(buf,sizeof(buf));
	bzero(&cliaddr,len);
	recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&cliaddr,&len);
	printf("%s:%s",inet_ntoa(cliaddr.sin_addr),buf);
}

int main(int argc,char *argv[])  // ./server 50002
{
	//1. 创建udp套接字
	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	//2. 绑定IP地址到套接字上
	struct sockaddr_in srvaddr;
	len = sizeof(srvaddr);
	bzero(&srvaddr,len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(argv[1]));
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr *)&srvaddr,len);

	//3. 捕捉信号
	signal(SIGIO,my_fun);

	//4. 设置套接字的属主
	fcntl(sockfd,F_SETOWN,getpid());

	//5. 激活信号驱动IO
	int state;
	state = fcntl(sockfd,F_GETFL);
	state |= O_ASYNC;
	fcntl(sockfd,F_SETFL,state);

	//6. 坐等别人写信给我
	while(1)
		pause();
}





