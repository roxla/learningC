#include "thread_pool.h"

bool init_pool(thread_pool *pool, unsigned int threads_number)
{
	//1. 初始化互斥锁
	pthread_mutex_init(&pool->lock, NULL);
	
	//2. 初始化条件变量
	pthread_cond_init(&pool->cond, NULL);

	//3. 初始化线程池状态为开启状态
	pool->shutdown = false;
	
	//4. 为任务队列的头节点申请堆空间
	pool->task_list = malloc(sizeof(struct task));
	
	//5. 为线程池中线程的id号储存空间申请堆区空间
	pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);

	//6. 错误判断
	if(pool->task_list == NULL || pool->tids == NULL)
	{
		perror("allocate memory error");
		return false;
	}

	//7. 为任务队列的头节点的指针域赋值为NULL
	pool->task_list->next = NULL;
	
	//8. 设置当前线程池最大等待任务个数为1000个。
	pool->max_waiting_tasks = MAX_WAITING_TASKS;
	
	//9. 设置当前线程池等待任务个数为0。
	pool->waiting_tasks = 0;
	
	//10. 初始化当前线程池线程的个数。
	pool->active_threads = threads_number;

	//11. 创建线程
	int i;
	for(i=0; i<pool->active_threads; i++)
	{
		if(pthread_create(&((pool->tids)[i]), NULL,routine, (void *)pool) != 0)
		{
			perror("create threads error");
			return false;
		}
	}

	//12. 初始化成功
	return true;
}