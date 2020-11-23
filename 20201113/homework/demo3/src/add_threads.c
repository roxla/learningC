
#include "thread_pool.h"

int add_thread(thread_pool *pool, unsigned additional_threads)
{
	//1. 如果说添加0条线程，则直接返回0
	if(additional_threads == 0)
		return 0; 

	//2. 计算了一下添加之后总线程条数
	unsigned total_threads = pool->active_threads + additional_threads;
					
	int i, actual_increment = 0;

	//3. 添加线程
	for(i = pool->active_threads;    i < total_threads && i < MAX_ACTIVE_THREADS;     i++) 
	{
		if(pthread_create(&((pool->tids)[i]),NULL, routine, (void *)pool) != 0)
		{
			perror("add threads error");

			//4. 一条都创建不了，就返回-1。
			if(actual_increment == 0) 
			{
				return -1;
			}
			break;
		}

		//实际创建的条数
		actual_increment++;  
	}

	//4. 当前的线程个数 = 之前线程个数 + 实际创建的条数
	pool->active_threads += actual_increment; 

	//5. 返回实际创建的条数。
	return actual_increment; 
}