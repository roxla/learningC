#include "head.h"

// 登录
pt_uinfo user_login(int connfd)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    // tips
    char tip_err[] = "Input error!\n";
    char tip_many[] = "Input to many!\n";
    char tip_succ[] = "Wecome airplane system!\n\n";

    int ret, count = 0;
    pt_uinfo temp;

    // 检查连接人数

    if (!max_links())
    {
        snprintf(tip_cli, sizeof(tip_cli), "To many links, pleae wait some time!\n");
        send(connfd, tip_cli, strlen(tip_cli), 0);
        return NULL;
    }

    // 账号输入
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入用户名(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        // 判断信息是否一致
        temp = check_user_list(buf, "name");
        if (!temp)
        {
            // 输错三次退出
            count++;
            send(connfd, tip_err, strlen(tip_err), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return NULL;
            }
        }
        else
        {
            if (temp->connfd == -1)
            {
                count = 0;
                break;
            }
            else
            {
                snprintf(tip_cli, sizeof(tip_cli), "该账户已登录!\n");
                send(connfd, tip_cli, strlen(tip_cli), 0);
                return NULL;
            }
        }
    }
    // 密码输入
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入密码(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        // 判断信息是否一致
        if (!strncmp(buf, temp->pwd, strlen(temp->pwd)))
        {
            send(connfd, tip_succ, strlen(tip_succ), 0);
            break;
        }
        else
        {
            // 输错三次退出
            count++;
            send(connfd, tip_err, strlen(tip_err), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return NULL;
            }
        }
    }
    // 将用户套接字写入
    temp->connfd = connfd;
    // 连接数+1
    boss->count++;

    return temp;
}

// 注册
void user_reg(int connfd)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret, count = 0;
    char tip_many[] = "Input to many!\n";
    pt_uinfo new_node, temp;

    // 初始化一个新节点
    new_node = reg_init();

    // 用户名 //不能重复
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入用户名(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            free(new_node);
            quit_user(connfd);
        }
        if ((strlen(buf) - 1) > NAME_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的用户名过长!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        // 判断信息
        temp = check_user_list(buf, "name");
        if (temp) // 相同
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "已有该用户名!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else // 不同
        {
            memcpy(new_node->name, buf, strlen(buf) - 1); // 去掉\n
            count = 0;
            break;
        }
    }
    // 密码
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入密码(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            free(new_node);
            quit_user(connfd);
        }
        if ((strlen(buf) - 1) > PWD_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的密码过长!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            memcpy(new_node->pwd, buf, strlen(buf) - 1); // 去掉\n
            count = 0;
            break;
        }
    }
    // 手机号 //不能重复
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入手机号(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            free(new_node);
            quit_user(connfd);
        }
        if ((strlen(buf) - 1) > TEL_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的手机号过长!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        // 判断信息
        temp = check_user_list(buf, "tel");
        if (temp) // 相同
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "已有该手机号!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else // 不同
        {
            memcpy(new_node->tel, buf, strlen(buf) - 1); // 去掉\n
            count = 0;
            break;
        }
    }
    // 身份证号 //不能重复
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入身份证(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            free(new_node);
            quit_user(connfd);
        }
        if ((strlen(buf) - 1) > ID_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "输入的身份证过长!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        // 判断信息
        temp = check_user_list(buf, "id");
        if (temp) // 相同
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "已有该身份证!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else // 不同
        {
            memcpy(new_node->id, buf, strlen(buf) - 1); // 去掉\n
            count = 0;
            break;
        }
    }

    //访问之前,要上锁
    pthread_mutex_lock(&m);

    // 修改指向
    list_add_tail(&new_node->list, &boss->head->list);

    //访问链表后要解锁
    pthread_mutex_unlock(&m);

    snprintf(tip_cli, sizeof(tip_cli), "注册成功,请登录\n");
    send(connfd, tip_cli, strlen(tip_cli), 0);
}

// 初始化注册链表
pt_uinfo reg_init()
{
    // 申请一个堆空间
    pt_uinfo new_node = malloc(sizeof(st_uinfo));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return NULL;
    }
    // 将数据给入新节点的数据域
    // root
    new_node->isroot = 0;
    // friend_list_init
    new_node->myFriend = friend_init(new_node);
    // flight_list_init
    new_node->myflg = flight_init(new_node);
    // cash
    new_node->cash = 0;
    // vip
    new_node->vip = 0;
    // twitter_list_init
    new_node->myTwitter = twitter_init(new_node);

    return new_node;
}

// 修改密码
void change_pwd(int connfd)
{
    char buf[MAX_IO];
    char tip_cli[MAX_IO];
    int ret, count = 0;
    char tip_many[] = "Input to many!\n";
    pt_uinfo new_node, temp;

    // 获取用户输入的用户名
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入用户名(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 接收用户输入
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        // 遍历链表，查找用户名，次数3次
        temp = check_user_list(buf, "name");
        if (!temp)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "无此用户名!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            if (temp->isroot == 1)
            {
                // 管理员账户禁止修改
                snprintf(tip_cli, sizeof(tip_cli), "该账户为管理员账户,不能修改!\n");
                send(connfd, tip_cli, strlen(tip_cli), 0);
                return;
            }
            else if (temp->connfd == -1 || temp->connfd == connfd)
            {
                // 成功
                count = 0;
                break;
            }
            else
            {
                snprintf(tip_cli, sizeof(tip_cli), "该账户使用中!\n");
                send(connfd, tip_cli, strlen(tip_cli), 0);
                return;
            }
        }
    }
    // 找到后获取数据中的手机号
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入手机号(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 获取用户输入的手机号
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        // 对比
        if (strncmp(buf, temp->tel, strlen(buf) - 1) || (strlen(buf) - 1) != strlen(temp->tel))
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "手机号不正确!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            // 成功
            count = 0;
            break;
        }
    }
    // 获取用户输入的密码
    while (1)
    {
        bzero(buf, sizeof(buf));
        snprintf(tip_cli, sizeof(tip_cli), "请输入密码(剩余次数%d次):\n", 3 - count);
        // 发送提示
        send(connfd, tip_cli, strlen(tip_cli), 0);
        // 获取用户输入的密码
        ret = recv(connfd, buf, sizeof(buf), 0);
        // 超时处理
        if (ret == -1)
        {
            quit_user(connfd);
        }
        // 对比
        if (!strncmp(buf, temp->pwd, strlen(buf) - 1) && (strlen(buf) - 1) == strlen(temp->pwd))
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "新密码与旧密码一致!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
            else
            {
                // 失败 重新输入（3次）
            }
        }
        else if ((strlen(buf) - 1) > PWD_MAX_LEN)
        {
            // 输错三次退出
            count++;
            snprintf(tip_cli, sizeof(tip_cli), "密码输入过长!\n");
            send(connfd, tip_cli, strlen(tip_cli), 0);
            if (count >= 3)
            {
                send(connfd, tip_many, strlen(tip_many), 0);
                return;
            }
        }
        else
        {
            //访问之前,要上锁
            pthread_mutex_lock(&m);

            // 修改对应链表中的密码项
            memcpy(temp->pwd, buf, strlen(buf) - 1); // 去掉\n

            //访问链表后要解锁
            pthread_mutex_unlock(&m);
            break;
        }
    }
    snprintf(tip_cli, sizeof(tip_cli), "修改成功,请登录!\n\n");
    send(connfd, tip_cli, strlen(tip_cli), 0);

    return;
}

// 检查用户信息
pt_uinfo check_user_list(char *input, char *type)
{
    pt_uinfo get_node, n;
    int check = 0;
    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {
        if (!strcmp(type, "name"))
        {
            if ((strlen(input) - 1) == strlen(get_node->name))
            {
                if (!strncmp(input, get_node->name, strlen(input) - 1))
                {
                    check = 1;
                    break;
                }
            }
        }
        else if (!strcmp(type, "tel"))
        {
            if ((strlen(input) - 1) == strlen(get_node->tel))
            {
                if (!strncmp(input, get_node->tel, strlen(input) - 1))
                {
                    check = 1;
                    break;
                }
            }
        }
        else if (!strcmp(type, "id"))
        {
            if ((strlen(input) - 1) == strlen(get_node->id))
            {
                if (!strncmp(input, get_node->id, strlen(input) - 1))
                {
                    check = 1;
                    break;
                }
            }
        }
    }
    printf("hello2\n");
    if (check)
    {
        return get_node;
    }
    else
    {
        return NULL;
    }
}

// 单用户退出登录
void quit_user(int connfd)
{
    pt_uinfo get_node, n;

    //访问之前,要上锁
    pthread_mutex_lock(&m);

    list_for_each_entry_safe(get_node, n, &boss->head->list, list)
    {
        if (get_node->connfd == connfd)
        {
            get_node->connfd = -1;
        }
    }
    //访问链表后要解锁
    pthread_mutex_unlock(&m);

    send(connfd, "quit\n", 5, 0);
    close(connfd);
    printf("user exit\n");
    // 将用户信息存入文件中
    add_info_file(boss->head);
    if (boss->count)
    {
        boss->count--;
    }

    pthread_exit(NULL);
}

// 服务器退出
void quit_server()
{
    printf("\n=========== server quit =============\n");
    // 将用户信息存入文件中
    add_info_file(boss->head);
    // 将航班信息存入文件中
    add_flight_file(p_manager->head);
    plane_list_free();

    //访问之前,要上锁
    pthread_mutex_lock(&m);

    boss_list_free();

    //访问链表后要解锁
    pthread_mutex_unlock(&m);

    // 退出进程
    exit(0);
}

// 最大连接数限制
int max_links()
{
    if (boss->count >= MAX_LINKS)
        return 0;
    else
        return 1;
}
