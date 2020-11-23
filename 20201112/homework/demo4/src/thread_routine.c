#include "thread_pool.h"

void handler(void *arg)
{
	//1. 先输出被取消的线程的id号。
	printf("[%u] is ended.\n",
		(unsigned)pthread_self());

	//2. 解锁
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *routine(void *arg)
{
	//1. 先接住线程池地址
	thread_pool *pool = (thread_pool *)arg;
	struct task *p;

	//2. 进入一个死循环
	while(1)
	{
		//3. 将来线程收到了取消请求，那么就会执行handler这个函数
		pthread_cleanup_push(handler, (void *)&pool->lock);
		
		//4. 上锁
		pthread_mutex_lock(&pool->lock);
		
		//5. 如果当前线程池没有任务做并且线程池处于开启的状态
		while(pool->waiting_tasks == 0 && !pool->shutdown)
		{
			//6. 就进入条件变量中睡眠
			pthread_cond_wait(&pool->cond, &pool->lock); 
		}
		
		//6. 线程运行到这里，先做一个判断：
		//1）如果线程池开启状态，有任务      -> 线程就会做任务
		//2）如果线程池关闭状态，没有任务做  -> 线程就会解锁走人
		//3）如果线程池关闭状态，有任务      -> 线程继续做任务，直到waiting_tasks为0为止。

		//7. 如果当前线程池是关闭状态，并且没有任务做：
		if(pool->waiting_tasks == 0 && pool->shutdown == true)
		{	
			//8. 解锁
			pthread_mutex_unlock(&pool->lock);	
			
			//9. 走人
			pthread_exit(NULL); 
		}
		
		//10. 让指针p指向头节点的下一个节点
		p = pool->task_list->next;
		
		//11. 把指针p的下一个节点的地址赋值给头节点的指针。
		pool->task_list->next = p->next;
		
		//12. 当前等待的任务个数-1。
		pool->waiting_tasks--;

		//13. 拿完节点了，可以解锁
		pthread_mutex_unlock(&pool->lock);
		
		//14. 删除线程取消例程函数
		pthread_cleanup_pop(0);
		
		//15. 线程在执行任务函数之前，设置了不能响应取消。
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 
		
		//16. 执行任务函数
		(p->do_task)(p->arg);    
		
		//17. 做完任务之后，如果需要取消我，请随便。
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		//18. 释放掉做完的那个任务的节点
		free(p);
	}

	pthread_exit(NULL);
}



   