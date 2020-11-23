
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//静态初始化：
//pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void my_fun(void *arg)  //arg = m
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *fun(void *arg)
{
	pthread_mutex_t *m = (pthread_mutex_t *)arg;

	//将来我收到了取消请求时，我会先解锁，再退出。
	pthread_cleanup_push(my_fun,m);

	//做任务之前： 上锁
	pthread_mutex_lock(m);

	//任务：打印helloworld
	char str[] = "helloworld";
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		fprintf(stderr,"%c",str[i]);
		sleep(1);
	}

	//做完任务之后： 解锁
	pthread_mutex_unlock(m);

	//删除函数
	pthread_cleanup_pop(0);

	pthread_exit(NULL);
}

int main()
{
	pthread_mutex_t m;
	pthread_mutex_init(&m,NULL);

	//1. 产生4个线程
	int i;
	pthread_t tid[4];
	for(i=0;i<4;i++)
	{
		pthread_create(&tid[i],NULL,fun,(void *)&m);
	}

	//2. 接合线程
	for(i=0;i<4;i++)
	{
		pthread_join(tid[i],NULL);
	}

	//3. 销毁互斥锁
	pthread_mutex_destroy(&m);

	return 0;
}