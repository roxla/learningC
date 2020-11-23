#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int a = 100;  //临界资源
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *fun1(void *arg)  //读3s
{
	pthread_rwlock_rdlock(&rwlock);  //上读锁
	printf("thread1 rdlock lock!\n");
	printf("a = %d\n",a);            //打印a的值
	sleep(3);                        //共3s
	pthread_rwlock_unlock(&rwlock);  //解锁
	printf("thread1 rdlock unlock!\n");
}

void *fun2(void *arg)  //读5s
{
	pthread_rwlock_rdlock(&rwlock);  //上读锁
	printf("thread2 rdlock lock!\n");
	printf("a = %d\n",a);            //打印a的值
	sleep(5);                        //共5s
	pthread_rwlock_unlock(&rwlock);  //解锁
	printf("thread2 rdlock unlock!\n");
}

void *fun3(void *arg)  //写4s
{
	pthread_rwlock_wrlock(&rwlock);  //上写锁
	printf("thread3 wrlock lock!\n");
	a = 50;                          //修改a的值
	sleep(4);                        //共4s
	pthread_rwlock_unlock(&rwlock);  //解锁
	printf("thread3 wrlock unlock!\n");
}

void *fun4(void *arg)  //读5s
{
	pthread_rwlock_wrlock(&rwlock);  //上写锁
	printf("thread4 wrlock lock!\n");
	a = 1000;                        //修改a的值
	sleep(6);                        //共6s
	pthread_rwlock_unlock(&rwlock);  //解锁
	printf("thread4 wrlock unlock!\n");
}

void *fun_time(void *arg)
{
	int i;
	for(i=0;i<10000;i++)
	{
		printf("i = %d\n",i);
		sleep(1);
	}
}

int main()
{
	//0. 再开启一个用于计算时间的线程
	pthread_t tid;
	pthread_create(&tid,NULL,fun_time,NULL);

	//1. 创建2个线程
	pthread_t t1,t2,t3,t4;
	pthread_create(&t1,NULL,fun1,NULL); //读3s
	pthread_create(&t2,NULL,fun2,NULL); //读5s
	pthread_create(&t3,NULL,fun3,NULL); //写4s
	pthread_create(&t4,NULL,fun4,NULL); //写4s

	//2. 接合线程
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);

	//3. 销毁读写锁
	pthread_rwlock_destroy(&rwlock);

	return 0;
}