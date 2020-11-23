
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *fun(void *arg)
{
	pthread_t x = pthread_self();
	pthread_detach(x);

	int i;
	for(i=0;i<5;i++)
	{
		printf("child i = %d\n",i);
		sleep(1);
	}

	pthread_exit(NULL);
}

int main()
{
	//1. 定义一个属性变量
	pthread_attr_t attr;

	//2. 初始化该属性变量
	pthread_attr_init(&attr);

	//3. 设置分离属性给属性变量。
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//4. 使用该属性变量去创建一个线程。
	pthread_t tid;
	pthread_create(&tid,&attr,fun,NULL);

	//5. 3s后去接合线程。
	sleep(3);
	int ret;
	ret = pthread_join(tid,NULL);
	if(ret == 0)
	{
		printf("join success!\n");
	}
	else{
		printf("join error!\n");
	}

	//5. 销毁属性变量。
	pthread_attr_destroy(&attr);
	
	return 0;
}