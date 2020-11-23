#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
	//线程默认都是能响应的。
	//1. 设置线程为不能响应。
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

	//2. 让这个不能响应的状态持续一段时间
	int i;
	for(i=0;i<5;i++)
	{
		printf("thread disable cancel time:%d\n",i);
		sleep(1);  //在10s会收到一个取消请求，但是由于不能响应，所以不能提前
	}

	//3. 设置线程为能响应。
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);

	for(i=0;i<10;i++)
	{
		sleep(1);
		printf("thread enable cancel time:%d\n",i);  
	}

	return 0;
}

int main()
{
	//1. 先创建一个子线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun,NULL);

	//2. 主线程在3s发送一个取消请求给子线程
	sleep(3);
	pthread_cancel(tid);
	printf("main thread send cancel to child thread!\n");

	//3. 接合线程
	pthread_join(tid,NULL);

	return 0;
}