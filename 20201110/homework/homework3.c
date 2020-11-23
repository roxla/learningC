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

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
// int state = 0;

void* jack_snd(void* arg);
void* rose_rvc(void* arg);

int main(int argc, char const *argv[])
{
	key_t key = ftok(".",10);
	int shmid = shmget(key,1024,IPC_CREAT|0666);
	char *p = shmat(shmid,NULL,0);
	bzero(p,1024);

	pthread_t jack,rose;
	pthread_create(&jack,NULL,jack_snd,(void *)p);
	pthread_create(&rose,NULL,rose_rvc,(void *)p);

	pthread_join(jack,NULL);
	pthread_join(rose,NULL);

	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	pthread_mutex_destroy(&m);

	return 0;
}

void* jack_snd(void* arg)
{
	char *p = (char *)arg;
	while(1)
	{
		// if(state==0)
		// {
			usleep(10000);
			pthread_mutex_lock(&m);
			fgets(p,1024,stdin);
			// state = 1;
			pthread_mutex_unlock(&m);
			if(strncmp(p,"quit",4) == 0)
				break;
		// }
	}

	pthread_exit(NULL);
}

void* rose_rvc(void* arg)
{
	char *p = (char *)arg;
	while(1)
	{
		// if(state == 1)
		// {	
			usleep(11000);
			pthread_mutex_lock(&m);
			printf("from Jack:%s",p);
			// state = 0;
			pthread_mutex_unlock(&m);
			if(strncmp(p,"quit",4) == 0)
				break;
		// }
		
	}

	pthread_exit(NULL);
}