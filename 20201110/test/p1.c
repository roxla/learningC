
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void *fun(void *arg)
{
	//sem_t sem = *(sem_t *)arg;  //每一个线程会开辟新空间。
	sem_t *sem = (sem_t *)arg;  //每一个线程都指向原来的那个无名信号量。

	//抢叉子
	sem_wait(sem);

	char str[] = "helloworld";
	int i;
	for(i=0;str[i]!='\0';i++)
	{
		//printf("%c",str[i]); //默认输出对象： 标准输出（有缓冲区）
		fprintf(stderr,"%c",str[i]);  //指定内容输出到标准出错上。
		sleep(1);
	}

	//把手上的叉子放下
	sem_post(sem);

	pthread_exit(NULL);
}

int main()
{
	//0. 初始化无名信号量
	sem_t sem;
	sem_init(&sem,0,1);  //代表一开始有一把叉子

	//1. 创建4个线程
	//由于4个线程的任务是一样的，所以可以通过循环来创建。
	int i;
	pthread_t tid[4];
	for(i=0;i<4;i++)
	{
		pthread_create(&tid[i],NULL,fun,(void *)&sem);
	}

	//2、接合线程
	for(i=0;i<4;i++)
	{
		pthread_join(tid[i],NULL);
	}

	//3. 销毁无名信号量
	sem_destroy(&sem);

	return 0;
}