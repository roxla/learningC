#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

//jack: 负责将数据发送到共享内存上。

int main()
{
	//1. 申请key值。
	key_t key = ftok(".",10);

	//2. 根据key值去申请id号。
	int shmid = shmget(key,2048,IPC_CREAT|0666);

	//3. 根据id号去映射一片内存空间。
	char *p = shmat(shmid,NULL,0);

	//4. 从键盘中获取数据，然后将数据存放在共享内存中
	while(1)
	{
		fgets(p,2048,stdin);
		if(strncmp(p,"quit",4) == 0)
		{
			break;
		}
	}

	return 0;

}