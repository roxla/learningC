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

int msgid;

void *fun(void *arg)
{
	talk_buf r_buf;
	int ret;
	while(1)
	{
		bzero(&r_buf,sizeof(r_buf));
		ret = msgrcv(msgid,&r_buf,sizeof(r_buf.mtext),10,0);
		if(ret == -1)
		{
			exit(-1); //线程异常退出
		}

		//子进程将收到的数据打印出来
		printf("from main:%s", r_buf.mtext);
		
		// 睡眠1秒
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	//1. 创建消息队列key值。
	key_t key;
	key = ftok(".",10);
	//2. 根据key值去申请id号。
	msgid = msgget(key,IPC_CREAT|0666);
	// 3.创建一个线程
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, fun, NULL);
	if(ret != 0)
	{
		exit(-1);
	}
	// 接下来，这个进程称之为主线程，会继续运行
	talk_buf w_buf;
	while(1)
	{
		//清空缓冲区
		bzero(&w_buf,sizeof(w_buf));
		w_buf.mtype = 10;
		fgets(w_buf.mtext,sizeof(w_buf.mtext),stdin);
		ret = msgsnd(msgid,&w_buf,strlen(w_buf.mtext),0);
		if(ret == -1)
		{
			exit(-1); //主线程异常退出
		}

		//如果主线程发送了quit给Rose，退出
		if(strncmp(w_buf.mtext,"quit",4) == 0) 
		{
			//删除消息队列
			msgctl(msgid,IPC_RMID,NULL);

			//父进程退出
			exit(0);
		}
	}

	return 0;
}