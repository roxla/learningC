#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>
#include <stdlib.h>

sem_t *sem = NULL;
int shmid;

void *snd_fun(void *arg)
{
	char *p = (char *)arg;
	while(1)
	{
		fgets(p,1024,stdin);  //rhello
		sem_post(sem);
		
		if(strncmp(p+1,"quit",4) == 0)
		{
			exit(0);
		}
	}
}

void *rcv_fun(void *arg)
{
	char *p = (char *)arg;
	while(1)
	{
		//printf("p[0] = %c\n",p[0]);
		//printf("p[0] = %c\n",p[0]);
		if(p[0] == 'j')
		{
			sem_wait(sem);
			printf("from shm:%s",p+1);
			
			if(strncmp(p+1,"quit",4) == 0)
			{
				shmdt(p);
				shmctl(shmid,IPC_RMID,NULL);
				exit(0);
			}
			bzero(p,1024);
		}
	}
}


int main()
{
	//0. 有名信号量
	sem = sem_open("/sem_test",O_CREAT,0777,0);

	//1. 初始化共享内存
	key_t key = ftok(".",20);
	shmid = shmget(key,1024,IPC_CREAT|0666);
	char *p = shmat(shmid,NULL,0);
	bzero(p,1024);

	//2. 创建两个子线程
	pthread_t t1,t2;
	pthread_create(&t1,NULL,snd_fun,(void *)p);
	pthread_create(&t2,NULL,rcv_fun,(void *)p);

	//3. 接合线程
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}