
#include "thread_pool.h"
 
bool destroy_pool(thread_pool *pool)
{
	//1. 先设置线程池状态为关闭状态
	pool->shutdown = true; 
	
	//2. 唤醒所有小孩起来走人了
	pthread_cond_broadcast(&pool->cond);  
	
	//3. 接合所有的小孩
	int i;
	for(i=0; i<pool->active_threads; i++)
	{
		errno = pthread_join(pool->tids[i], NULL);

		if(errno != 0)
		{
			printf("join tids[%d] error: %s\n",
					i, strerror(errno));
		}
	
		else
			printf("[%u] is joined\n", (unsigned)pool->tids[i]);
		
	}


	//4. 释放链表头的空间。
	free(pool->task_list);  

	//5. 释放储存线程id号的空间。
	free(pool->tids);

	//6. 释放线程池的结构体空间。
	free(pool);

	//7. 销毁成功
	return true;
}