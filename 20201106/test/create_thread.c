

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
//目标：尝试接合一个新的子线程
//效果：主线程做完任务后，会不会等待子线程？

int state = 10;
//线程的例程函数

void *fun(void *arg)  // arg = &a
{
	printf("a = %d\n",*(int*)arg);
	//3. 子线程也会继续运行。
	int i;
	for(i=0;i<10;i++)
	{
		printf("child thread print apple!\n");
		sleep(1);
	}

	//4. 将自己的退出值返回到主线程
	//pthread_exit(&state); -> void *
	return &state;
}

int main()
{
	/* 单进程 */
	printf("helloworld!\n");

	//1. 创建一个新的子线程
	pthread_t tid;
	int ret;
	int a = 20;
	ret = pthread_create(&tid,NULL,fun,&a);
	if(ret == 0)
	{
		printf("pthread_create success!\n");
	}
	else{
		printf("pthread_create error!\n");
	}

	//2. 接下来，这个进程称之为主线程，会继续运行。
	int i;
	for(i=0;i<5;i++)
	{
		printf("main thread print hello!\n");
		sleep(1);
	}

	//3. 主线程需要接合子线程
	void *p = NULL;
	ret = pthread_join(tid,&p);  //p = &state
	printf("exit state = %d\n",*(int*)p);
	if(ret == 0)
	{
		printf("pthread_join success!\n");
	}
	else{
		printf("pthread_join error!\n");
	}

	return 0;
}

/*
exit(0);   int 

int p
wait(&p);  // p = 0

pthread_exit(&state);   void *

void *p
pthread_join(&p)   p = &state
*/
