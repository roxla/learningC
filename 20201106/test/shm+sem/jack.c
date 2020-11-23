#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>

//jack: 负责将数据发送到共享内存上。

int main()
{
	//1. 申请key值。
	key_t key1 = ftok(".",10);  //共享内存
	key_t key2 = ftok(".",20);  //信号量

	//2. 根据key值去申请id号。
	int shmid = shmget(key1,2048,IPC_CREAT|0666); //共享内存
	int semid = semget(key2,2,IPC_CREAT|0666);    //信号量

	//3. 根据id号去映射一片内存空间。
	char *p = shmat(shmid,NULL,0);

	//3.5 信号量设置起始值
	semctl(semid,0,SETVAL,1); //空间:1
	semctl(semid,1,SETVAL,0); //数据:0

	//空间p操作
	struct sembuf space;
	space.sem_num = 0;
	space.sem_op = -1;
	space.sem_flg = 0;

	//数据v操作
	struct sembuf data;
	data.sem_num = 1;
	data.sem_op = 1;
	data.sem_flg = 0;

	//4. 从键盘中获取数据，然后将数据存放在共享内存中
	while(1)
	{
		//请问空间能不能减1？
		semop(semid,&space,1);

		//如果能  -> 代码继续运行。
		//如果不能-> 阻塞等待

		fgets(p,2048,stdin); //把车开进去

		//信号量数据就会+1。
		semop(semid,&data,1);

		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}

		
	}

	return 0;

}