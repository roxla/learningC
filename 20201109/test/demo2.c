#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//子线程收到一个取消请求后，不要马上响应，先打印一句话"I recv cancel"，再响应取消。

void cancel_fun(void *arg)
{
	/* 释放资源的动作 */
	printf("I recv cancel!\n");
}

void *fun(void *arg)
{
	//1. 先告诉系统，将来收到取消请求，先执行cancel_fun
	pthread_cleanup_push(cancel_fun,NULL);

	//2. 让子线程持续一段时间
	int i;
	for(i=0;i<10;i++)
	{
		printf("i = %d\n",i);
		sleep(1);
	}

	//pthread_exit(NULL);
	return NULL;

	//3. 删除函数。
	pthread_cleanup_pop(0);

	return 0;
}

int main()
{
	//1. 先创建一个子线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun,NULL);

	//2. 3s后，发送一个取消请求给子线程
	//sleep(3);
	//pthread_cancel(tid);

	//3. 接合线程
	pthread_join(tid,NULL);

	return 0;
}