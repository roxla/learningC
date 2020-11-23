
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

//rose: 负责1s打印一次共享内存上的数据

int main()
{
	//1. 申请key值。
	key_t key = ftok(".",10);

	//2. 根据key值去申请id号。
	int shmid = shmget(key,2048,IPC_CREAT|0666);

	//3. 根据id号去映射一片内存空间。
	char *p = shmat(shmid,NULL,0);

	//4. 不断打印共享内存上的数据
	while(1)
	{
		printf("from shm:%s",p);
		sleep(1);

		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}
	}

	//5. 撤销映射
	shmdt(p);

	//6. 删除共享内存的对象
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}