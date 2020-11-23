
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

//rose: 负责1s打印一次共享内存上的数据

int main()
{
	//0. 创建并打开有名信号量。
	sem_t *sem = NULL;
	sem = sem_open("/sem_test",O_CREAT,0777,0);

	//1. 申请key值。
	key_t key = ftok(".",10);

	//2. 根据key值去申请id号。
	int shmid = shmget(key,2048,IPC_CREAT|0666);

	//3. 根据id号去映射一片内存空间。
	char *p = shmat(shmid,NULL,0);

	//4. 不断打印共享内存上的数据
	while(1)
	{
		//请问能不能数据-1？
		sem_wait(sem);
		//如果能  -> 函数返回
		//如果不能-> 阻塞等待

		printf("from shm:%s",p);
		

		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}
	}

	//5. 撤销映射
	shmdt(p);

	//6. 删除共享内存的对象
	shmctl(shmid,IPC_RMID,NULL);

	//7. 关闭有名信号量
	sem_close(sem);

	//8. 删除有名信号量
	sem_unlink("/sem_test");

	return 0;
}