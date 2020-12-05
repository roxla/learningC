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

bool add_task(thread_pool *pool,void *(*do_task)(void *arg), void *arg)
{
	//1. 为新任务的节点申请空间。
	struct task *new_task = malloc(sizeof(struct task));
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return false;
	}
	
	//2. 如果申请成功，就为数据域与指针域赋值：
	new_task->do_task = do_task;  
	new_task->arg = arg; 
	new_task->next = NULL;   //新节点的指针域肯定为NULL。
	
	//3. 修改/访问任务队列之前，都要上锁。
	pthread_mutex_lock(&pool->lock);
	
	//4. 当前线程池等待处理的任务个数是不是大于等于1000?
	if(pool->waiting_tasks >= MAX_WAITING_TASKS)
	{
		//5. 解锁
		pthread_mutex_unlock(&pool->lock);
		
		//6. 输出一个错误提示： 太多任务了！
		fprintf(stderr, "too many tasks.\n");
		
		//7. 将刚才申请的新节点的堆区空间释放掉。
		free(new_task);

		//8. 添加任务失败
		return false;
	}

	//5. 如果不是大于等于1000,说明可以添加：
	//6. 寻找链表中的最后一个节点
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;

	//从循环出来时.tmp就是指向最后一个节点。
	//7. 让最后一个节点的指针域指向新节点。
	tmp->next = new_task;
	
	//8. 当前等待任务的个数+1
	pool->waiting_tasks++;

	//9. 解锁
	pthread_mutex_unlock(&pool->lock);

	//10. 添加了任务，就随机唤醒条件变量中的一个线程起来工作。
	pthread_cond_signal(&pool->cond);
	
	//11. 添加任务成功，返回true。
	return true;
}

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
