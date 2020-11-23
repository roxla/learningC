#include "thread_pool.h"


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
