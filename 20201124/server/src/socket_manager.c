#include "head.h"

// 登录界面
void login_view_options(int connfd)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/login";
    int num, loop = 1;
    // 存放用户结构体
    pt_uinfo user;
    while (loop)
    {
        while (send_view(connfd, viwe_path))
            ;
        num = num_msg(connfd);
        switch (num)
        {
        case 1:
            // 登录
            if (user = user_login(connfd))
                loop = 0;
            break;
        case 2:
            // 注册
            user_reg(connfd);
            break;
        case 3:
            // 修改密码
            change_pwd(connfd);
            break;
        default:
            // 退出
            quit_user(connfd);
            break;
        }
    }
    if (user->isroot == 1)
    {
        main_root_options(connfd, user);
    }
    else
    {
        main_view_options(connfd, user);
    }
}

// 用户主界面
void main_view_options(int connfd, pt_uinfo user)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/user/main";
    int num, loop = 1;
    char user_info[100];
    // 功能选项
    while (loop)
    {
        snprintf(user_info, sizeof(user_info), "当前用户:%s    是否为vip:%d\n", user->name, user->vip);
        send(connfd, user_info, strlen(user_info), 0);
        while (send_view(connfd, viwe_path))
            ;
        num = num_msg(connfd);
        switch (num)
        {
        case 1: // 航班列表
            view_flight_list(user);
            break;
        case 2: // 查询航班
            search_fligth_list(user);
            break;
        case 3: // 快速查询
            quick_search(user);
            break;
        case 4: // 订购航班
            user_flight_buy(user);
            break;
        case 5: // 已购航班
            user_flight_list(user);
            break;
        case 6: // 退票改签
            flight_manager(user);
            break;
        case 7: // 查询余额
            search_cash(user);
            break;
        case 8: // 充值账户
            charge_cash(user);
            break;
        case 9: // 修改信息
            user_change_main(user);
            break;
        case 10: // 聊天功能
            friend_send_main(user);
            break;
        case 11: // 取票
            user_plane_get(user);
            break;
        default: // 退出
            quit_user(connfd);
            break;
        }
    }
}

// 管理员主界面
void main_root_options(int connfd, pt_uinfo user)
{
    char viwe_path[] = "/mnt/hgfs/roxla/C/20201124/server/file/view/root/main";
    int num, loop = 1;
    char user_info[40];
    snprintf(user_info, sizeof(user_info), "当前用户: 管理员\n");
    // 功能选项
    while (loop)
    {
        send(connfd, user_info, strlen(user_info), 0);
        while (send_view(connfd, viwe_path))
            ;
        num = num_msg(connfd);
        switch (num)
        {
        case 1: // 航班列表
            view_flight_list(user);
            break;
        case 2: // 查询航班
            search_fligth_list(user);
            break;
        case 3: // 快速查询
            quick_search(user);
            break;
        case 4: // 录入航班
            flight_add(user);
            break;
        case 5: // 删除航班
            flight_del(user);
            break;
        default: // 退出登录
            quit_user(connfd);
            break;
        }
    }
}