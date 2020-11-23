#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <pthread.h>

sem_t sem;

void* jack_snd(void* arg);
void* rose_rvc(void* arg);

int main(int argc, char const *argv[])
{
	key_t key = ftok(".",10);
	int shmid = shmget(key,1024,IPC_CREAT|0666);
	char *p = shmat(shmid,NULL,0);
	bzero(p,1024);

	sem_init(&sem,0,0);

	pthread_t jack,rose;
	pthread_create(&jack,NULL,jack_snd,(void *)p);
	pthread_create(&rose,NULL,rose_rvc,(void *)p);

	pthread_join(jack,NULL);
	pthread_join(rose,NULL);

	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	sem_destroy(&sem);

	return 0;
}

void* jack_snd(void* arg)
{
	char *p = (char *)arg;
	while(1)
	{
		fgets(p,1024,stdin);
		sem_post(&sem);
		if(strncmp(p,"quit",4) == 0)
			break;
	}

	pthread_exit(NULL);
}

void* rose_rvc(void* arg)
{
	char *p = (char *)arg;
	while(1)
	{
		sem_wait(&sem);
		printf("from Jack:%s",p);
		if(strncmp(p,"quit",4) == 0)
			break;
	}

	pthread_exit(NULL);
}