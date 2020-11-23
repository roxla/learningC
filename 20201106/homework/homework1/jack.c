#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct my_buf
{
	long mtype;     //消息类型
	char mtext[50]; //消息正文
} talk_buf;

#define J2R 10
#define R2J 20

int state = 10;

void *fun(void *arg)
{
	talk_buf r_buf;
	int ret;
	while(1)
	{
		bzero(&r_buf,sizeof(r_buf));
		ret = msgrcv(*(int *)arg,&r_buf,sizeof(r_buf.mtext),R2J,0);
		if(ret == -1)
		{
			exit(-1); //线程异常退出
		}

		//子进程将收到的数据打印出来
		printf("from Rose:%s", r_buf.mtext);
		if(strncmp(r_buf.mtext,"byby",4) == 0)
		{
			//删除消息队列
			msgctl(*(int *)arg,IPC_RMID,NULL);

			//退出。
			exit(0); //正常退出
		}
	}
	
}

int main(int argc, char const *argv[])
{
	//1. 创建消息队列key值。
	key_t key;
	key = ftok(".",10);

	//2. 根据key值去申请id号。
	int msgid;
	msgid = msgget(key,IPC_CREAT|0666);
	// 3.创建一个线程
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, fun, &msgid);
	if(ret != 0)
	{
		exit(-1);
	}
	// 接下来，这个进程称之为主线程，会继续运行
	// 4.发送消息
	talk_buf w_buf;
	while(1)
	{
		//清空缓冲区
		bzero(&w_buf,sizeof(w_buf));
		w_buf.mtype = J2R;
		fgets(w_buf.mtext,sizeof(w_buf.mtext),stdin);
		ret = msgsnd(msgid,&w_buf,strlen(w_buf.mtext),0);
		if(ret == -1)
		{
			exit(-1); //主线程异常退出
		}

		//如果主线程发送了quit给Rose，退出
		if(strncmp(w_buf.mtext,"byby",4) == 0) 
		{

			//父进程退出
			exit(0);
		}
	}

	return 0;
}
