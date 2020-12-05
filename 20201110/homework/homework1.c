#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int a = 200;

void *get_money(void *arg);

int main(int argc, char const *argv[])
{
	pthread_t thread[4];

	int i;
	for (i = 0; i < 4; i++)
	{
		pthread_create(&thread[i], NULL, get_money, NULL);
	}

	sleep(5);
	pthread_mutex_lock(&m);
	a += 400;
	printf("a增加了400\n", );
	pthread_mutex_unlock(&m);
	sleep(2);
	pthread_cond_broadcast(&cond);

	sleep(3);
	pthread_mutex_lock(&m);
	a += 200;
	printf("a增加了200\n", );
	pthread_mutex_unlock(&m);
	sleep(2);
	pthread_cond_signal(&cond);

	for (i = 0; i < 4; i++)
	{
		pthread_join(thread[i], NULL);
	}

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&cond);

	return 0;
}

void *get_money(void *arg)
{
	pthread_mutex_lock(&m);
	while (a < 200)
		pthread_cond_wait(&cond, &m);
	a -= 200;
	printf("线程已获取\n");

	pthread_mutex_unlock(&m);

	pthread_exit(NULL);
}