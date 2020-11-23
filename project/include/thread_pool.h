#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

struct task
{
	void *(*do_task)(void *arg);
	void *arg;

	struct task *next;
};

typedef struct thread_pool
{
	pthread_mutex_t lock; // 互斥锁
	pthread_cond_t  cond; // 条件变量
	bool shutdown; // 关闭线程池
	struct task *task_list; // 单向链表指针
	pthread_t *tids; // 线程指针
	unsigned max_waiting_tasks; // 最大任务数
	unsigned waiting_tasks; // 当前等待任务数
	unsigned active_threads; // 线程数
	unsigned num; // 编号数
}thread_pool;


bool init_pool(thread_pool *pool, unsigned int threads_number);
bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *task);
int  add_thread(thread_pool *pool, unsigned int additional_threads_number);
int  remove_thread(thread_pool *pool, unsigned int removing_threads_number);
bool destroy_pool(thread_pool *pool);

void *routine(void *arg);


#endif
