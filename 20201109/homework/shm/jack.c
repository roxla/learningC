#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

//jack: 负责将数据发送到共享内存上。

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

	//4. 从键盘中获取数据，然后将数据存放在共享内存中
	while(1)
	{
		//当前有名信号量值： 0

		fgets(p,2048,stdin);  //把车开进去

		//导致数据自动+1
		sem_post(sem);

		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}
	}

	return 0;

}