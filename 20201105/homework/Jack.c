#include "talk.h"

int main(int argc, char const *argv[])
{
    //1. 申请key值
    key_t key1 = ftok(".", 10);

    //2. 根据key值来申请id号
    int msgid = msgget(key1, IPC_CREAT | 0666);

    talk_buf w_buf;
    talk_buf r_buf;
    int ret;
    pid_t x;

    // 3.创建子进程
    x = fork();
    // 父进程发送消息
    if (x > 0)
    {
        while (1)
        {
            bzero(&w_buf, sizeof(w_buf));                   // 清空写缓冲区
            w_buf.mtype = J2R;                              // 设置消息的类型
            fgets(w_buf.mtext, sizeof(w_buf.mtext), stdin); // 从键盘获取消息的正文
            ret = msgsnd(msgid, &w_buf, strlen(w_buf.mtext), 0);
            if (ret == -1)
                break;

            if (!strncmp(w_buf.mtext, "Byby", 4))
            {
            }
        }
    }
    // 子进程接受消息
    if (x == 0) // 子进程
    {
        while (1)
        {
            bzero(&r_buf, sizeof(r_buf)); // 清空读缓冲区
            ret = msgrcv(msgid, &r_buf, sizeof(r_buf.mtext), R2J, 0);
            if (ret == -1)
                break;
            printf("from Rose:%s", r_buf.mtext);
        }
    }

    //4. 删除消息队列
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
