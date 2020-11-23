#include "thread_pool.h"

void *mytask(void *arg) //线程的任务
{
	int n = *(int*)arg;

	//工作任务：余数是多少，就睡多少秒，睡完，任务就算完成
	printf("[%u][%s] ==> job will be done in %d sec...\n",
		(unsigned)pthread_self(), __FUNCTION__, n);

	sleep(n);

	printf("[%u][%s] ==> job done!\n",
		(unsigned)pthread_self(), __FUNCTION__);

	return NULL;
}

void *count_time(void *arg)
{
	int i = 0;
	while(1)
	{
		sleep(1);
		printf("sec: %d\n", ++i);
	}
}

int main(void)
{
	// 本线程用来显示当前流逝的秒数
	// 跟程序逻辑无关
	pthread_t a;
	pthread_create(&a, NULL, count_time, NULL);

	// 1, initialize the pool
	thread_pool *pool = malloc(sizeof(thread_pool));
	init_pool(pool, 2);
	//2个线程都在条件变量中睡眠

	// 2, throw tasks
	printf("throwing 3 tasks...\n");
	//随机了3个数字
	int aa = (rand()%10);
	int b = (rand()%10);
	int c = (rand()%10);
	
	add_task(pool, mytask, (void *)&aa);
	add_task(pool, mytask, (void *)&b);
	add_task(pool, mytask, (void *)&c);

	// 3, check active threads number
	printf("current thread number: %d\n",
			remove_thread(pool, 0));//2
	sleep(9);

	// 4, throw tasks
	printf("throwing another 6 tasks...\n");
	
	int d,e,f,g,h,j;
	d = (rand()%10);
	e = (rand()%10);
	f = (rand()%10);
	g = (rand()%10);
	h = (rand()%10);
	j = (rand()%10);
	
	add_task(pool, mytask, (void *)&d);
	add_task(pool, mytask, (void *)&e);
	add_task(pool, mytask, (void *)&f);
	add_task(pool, mytask, (void *)&g);
	add_task(pool, mytask, (void *)&h);
	add_task(pool, mytask, (void *)&j);
	
	// 5, add threads
	add_thread(pool, 2);

	sleep(10);

	// 6, remove threads
	printf("remove 3 threads from the pool, "
	       "current thread number: %d\n",
			remove_thread(pool, 3));

	// 7, destroy the pool
	destroy_pool(pool);
	return 0;
}
