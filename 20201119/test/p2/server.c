#include "head.h"

//管理客户端结构体
struct client{
	int clifd[20];  //存放那些连接到服务器上的客户端的已连接套接字
	int count;      //计算当前连接上服务器的人数
};

int init_cli(struct client *cli)
{
	int i;
	for(i=0;i<20;i++)
	{
		(cli->clifd)[i] = -1;
	}

	cli->count = 0;  //当前没有客户端连接上来

	return 0;
}

int add_cli(struct client *cli,int connfd)
{
	//1. 判断满人了没有
	if(cli->count >= 20)
	{
		return -1;  //满人了，添加失败
	}

	//2. 如果没有满人，则开始寻找空位(-1)
	int i;
	for(i=0;i<20;i++)
	{
		if( (cli->clifd)[i] == -1 )  //找到空位
		{
			(cli->clifd)[i] = connfd;
			break;
		}
	}

	//3. 当前连接的人数+1
	cli->count++;

	return 0;
}


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
	//1. 初始化客户端结构体
	struct client *cli = (struct client *)malloc(sizeof(struct client));
	init_cli(cli);

	//2. 初始化套接字
	int sockfd;
	sockfd = init_sock(argv[1]);

	//sockfd -> 监听套接字
	//3. 设置监听套接字为非阻塞属性
	int state;
	state = fcntl(sockfd,F_GETFL);
	state |= O_NONBLOCK;
	fcntl(sockfd,F_SETFL,state);

	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	int connfd,ret;
	int i;
	char buf[100];

	while(1)
	{
		//4. 不断非阻塞地等待客户端的连接
		bzero(&cliaddr,len);
		connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);

		//5. 如果真的有人连接，则将这个人的套接字塞到数组中
		if(connfd > 0)
		{
			//6. 先将这个套接字设置为非阻塞
			state = fcntl(connfd,F_GETFL);
			state |= O_NONBLOCK;
			fcntl(connfd,F_SETFL,state);

			//7. 将这个connfd存储到数组中
			ret = add_cli(cli,connfd);
			if(ret == -1)
			{
				printf("add usr error!\n");
				close(connfd);
			}

			//8. 输出一下是谁连接进来
			printf("new connfd:%d\n",connfd);
			printf("new connection:%s\n",inet_ntoa(cliaddr.sin_addr));
		}

		//6. 如果没有人连接，则开始询问所有连接服务器上的客户端有没有话说。
		for(i=0;i<20;i++)
		{
			bzero(buf,sizeof(buf));
			if( (cli->clifd)[i] == -1 )  //说明这个位置是空的!
			{
				continue;  //继续问下一个
			}

			//如果不是空位，而且又说话了
			if( recv((cli->clifd)[i],buf,sizeof(buf),0) >= 0)
			{
				printf("from %d client : %s",(cli->clifd)[i],buf);
				if(strncmp(buf,"quit",4) == 0)  //这个客户端想埋单走人
				{
					close((cli->clifd)[i]);
					(cli->clifd)[i] = -1;
					cli->count--;
				}
			}
			else{
				//不是空位，又不说话，继续询问下一个客户端。
				continue;
			}
		}

		//7. 20个客户端都问完了，就继续等待连接。
	}
}