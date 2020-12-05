#include "head.h"

// 入口函数
void user_change_main(pt_uinfo user)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/select1/change";
    int num, loop = 1;
    char user_info[100];
    snprintf(user_info, sizeof(user_info), "当前用户:%s    是否为vip:%d\n", user->name, user->vip);
    // 功能选项
    while (loop)
    {
        send(user->connfd, user_info, strlen(user_info), 0);
        while (send_view(user->connfd, viwe_path))
            ;
        num = num_msg(user->connfd);
        switch (num)
        {
        case 1: // 修改密码
            user_change_pwd(user);
            break;
        case 2: // 修改手机
            user_change_tel(user);
            break;
        case 3: // 修改身份证
            user_change_id(user);
            break;
        default: // 返回
            return;
            break;
        }
    }
}

// 修改手机号
void user_change_tel(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret, count = 0;
    char tip_many[] = "Input to many!\n";
    pt_uinfo temp;

    // 获取用户输入的手机号
    bzero(tip_cli, sizeof(tip_cli));
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入新的手机号(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        // 对比
        // 判断信息
        temp = check_user_list(buf, "tel");
        if (!strncmp(buf, user->tel, strlen(buf) - 1) && (strlen(buf) - 1) == strlen(user->tel)) // 判断新旧手机号是否相同
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "新手机号与旧手机号一致!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else if (temp) // 判断是否已有该手机号的注册信息
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "已有该手机号!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else if ((strlen(buf) - 1) > TEL_MAX_LEN) // 判断新手机号是否过长
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的手机号过长!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            //访问之前,要上锁
            pthread_mutex_lock(&m);

            // 修改对应链表中的密码项
            memcpy(user->tel, buf, strlen(buf) - 1); // 去掉\n
            snprintf(tip_cli, sizeof(tip_cli), "手机号修改成功!\n\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);

            //访问链表后要解锁
            pthread_mutex_unlock(&m);

            break;
        }
    }

    return;
}

// 修改密码
void user_change_pwd(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret, count = 0;
    char tip_many[] = "Input to many!\n";

    // 获取用户输入的密码
    bzero(tip_cli, sizeof(tip_cli));
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入新的密码(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        // 对比
        if (!strncmp(buf, user->pwd, strlen(buf) - 1) && (strlen(buf) - 1) == strlen(user->pwd))
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "新密码与旧密码一致!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else if ((strlen(buf) - 1) > PWD_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的密码过长!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            //访问之前,要上锁
            pthread_mutex_lock(&m);

            // 修改对应链表中的密码项
            memcpy(user->pwd, buf, strlen(buf) - 1); // 去掉\n
            snprintf(tip_cli, sizeof(tip_cli), "密码修改成功!\n\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);

            //访问链表后要解锁
            pthread_mutex_unlock(&m);
            
            break;
        }
    }

    return;
}

// 修改身份证
void user_change_id(pt_uinfo user)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret, count = 0;
    char tip_many[] = "Input to many!\n";
    pt_uinfo temp;

    // 获取用户输入的身份证
    bzero(tip_cli, sizeof(tip_cli));
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入新的身份证(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(user->connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(user->connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(user->connfd);
        }
        // 对比
        // 判断信息
        temp = check_user_list(buf, "id");
        if (!strncmp(buf, user->id, strlen(buf) - 1) && (strlen(buf) - 1) == strlen(user->id)) // 判断新旧身份证是否相同
        {
            // 输错三次退出
            snprintf(tip_cli, sizeof(tip_cli), "新身份证与旧身份证一致!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            count++;
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
            // 失败 重新输入（3次）
        }
        else if (temp) // 判断是否已有该身份证的注册信息
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "已有该身份证!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else if ((strlen(buf) - 1) > ID_MAX_LEN) // 判断新身份证是否过长
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的身份证过长!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(user->connfd, tip_many, strlen(tip_many), 0);
                return;
            }
            snprintf(tip_cli, sizeof(tip_cli), "输入的身份证过长!\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
        }
        else
        {
            // 修改对应链表中的密码项
            memcpy(user->id, buf, strlen(buf) - 1); // 去掉\n
            snprintf(tip_cli, sizeof(tip_cli), "身份证修改成功!\n\n");
            send(user->connfd, tip_cli, strlen(tip_cli), 0);
            break;
        }
    }

    return;
}