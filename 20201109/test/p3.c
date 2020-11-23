#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
	pthread_detach(pthread_self());

	int i;
	for(i=0;i<5;i++)
	{
		printf("child i = %d\n",i);
		sleep(1);
	}
}

int main()
{
	//1. 创建一个普通属性的线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun,NULL);  //非分离的

	//2. 设置线程的属性为分离属性。
	//pthread_detach(tid);

	//3. 尝试去接合线程
	int ret;
	sleep(2);  //确保pthread_detach()先运行。
	ret = pthread_join(tid,NULL);
	if(ret == 0)
	{
		printf("pthread join success!\n");
	}
	else{
		printf("pthread join error!\n");
	}

	return 0;
}