#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
	pthread_t tid = *(pthread_t *)arg;
	printf("child tid = %d\n",(int)tid);
	int i;
	for(i=0;i<5;i++)
	{
		printf("child i = %d\n",i);
		sleep(1);
	}
}

int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL,fun,(void *)&tid);
	printf("main tid = %d\n",(int)tid);

	sleep(2);
	pthread_cancel(tid);
	printf("I send cancel to child!\n");

	int ret;
	ret = pthread_join(tid,NULL);
	printf("ret = %d\n",ret);
	return 0;
}