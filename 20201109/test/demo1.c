#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
	//默认情况： 延迟响应
	//持续一段时间：不要遇到取消点
	int i,j;
	for(i=0;i<100000;i++)
	{
		for(j=0;j<100000;j++)
		{

		}
	}

	while(1)
	{
		fputc('a',stderr); //将字符a输出到标准出错   //取消点
		printf("helloworld!\n");                     //取消点
	}
}

int main()
{
	//1. 创建一个子线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun,NULL);

	//2. 马上发送取消请求
	pthread_cancel(tid);
	printf("I send cancel to child thread!\n");

	//3. 接合线程
	pthread_join(tid,NULL);

	return 0;
}