#include "thread_pool.h"


int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	//1. 如果删除0条，则返回线程池中剩余的条数
	if(removing_threads == 0)
		return pool->active_threads; 

	//2. 计算删除完之后的理论止
	int remaining_threads = pool->active_threads - removing_threads;
	/*
					2     =			 5           -      3
					-1	  = 		 5           -      6
					0     =          5           -      5
	*/
	
	//3. 线程池中剩余条数至少有1条。
	remaining_threads = remaining_threads > 0 ? remaining_threads : 1;

	//4. 依次将需要取消的线程取消掉。
	int i;  
	for(i=pool->active_threads-1; i>remaining_threads-1; i--)
	{	
		errno = pthread_cancel(pool->tids[i]); 
		if(errno != 0)
			break;
	}

	//5. 取消失败，返回-1
	if(i == pool->active_threads-1) 
		return -1; 
	else
	{
		//6. 取消成功，返回剩余的条数。
		pool->active_threads = i+1; 
		return i+1; 
	}
}
