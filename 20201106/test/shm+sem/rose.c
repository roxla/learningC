#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>


//rose: 负责1s打印一次共享内存上的数据

int main()
{
	//1. 为信号量和共享内存申请key值。
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

	//数据p操作
	struct sembuf data;
	data.sem_num = 1;
	data.sem_op = -1;
	data.sem_flg = 0;

	//空间v操作
	struct sembuf space;
	space.sem_num = 0;
	space.sem_op = 1;
	space.sem_flg = 0;

	//4. 不断打印共享内存上的数据
	while(1)
	{
		//请问数据能不能减1？ -> 数据p操作
		semop(semid,&data,1);

		//如果能  -> 代码继续运行。
		//如果不能-> 阻塞等待 

		printf("from shm:%s",p);
		
		//信号量的空间+1
		semop(semid,&space,1);


		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}
	}

	//5. 撤销映射
	shmdt(p);

	//6. 删除共享内存的对象
	shmctl(shmid,IPC_RMID,NULL);
	semctl(semid,0,IPC_RMID,0);

	return 0;
}