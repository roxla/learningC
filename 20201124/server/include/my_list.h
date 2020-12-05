#ifndef __MY_DLIST_H_
#define __MY_DLIST_H_

// 存放宏定义
#define MAX_LINKS 5     // 最大连接数
#define PWD_MAX_LEN 31  // 最大密码长度
#define NAME_MAX_LEN 31 // 最大用户名长度
#define ID_MAX_LEN 19   // 最大身份证长度
#define TEL_MAX_LEN 11  // 最大手机号长度
#define MAX_IO 60       //
#define MAX_PLANE 25974 // 最大航班数

// 存放全局变量
// 互斥锁
extern pthread_mutex_t m;
// 连接用户管理链表
extern struct manager *boss;
// 连接航班管理链表
extern struct plane_manager *p_manager;
// 飞机类型链表
extern struct plane *p_type;

// 存放定义的结构体
// 飞机类型
struct plane
{
    // 数据域
    // 编号
    int id;
    // 类型名
    char type[5];

    // 指针域
    struct list_head list; //（前后关系）
};

// 航班信息
typedef struct flight_list
{
    // 数据域
    // 航班号 唯一
    char code[20];
    // 余票
    int num;
    // 金额
    int cash;
    // 飞机类型
    char type[5];
    // 起飞时间
    char time[9];
    // 班期
    char date[13];
    // 起点站
    char s_plane[3];
    // 终点站
    char e_plane[3];

    // 指针域
    struct list_head list; //（前后关系）
} st_flg, *pt_flg;

// 航班管理结构体
struct plane_manager
{
    pt_flg head;
    int count;
};

// 用户购买的航班
typedef struct my_flight
{
    // 数据域
    char flight[20]; // 航班
    int o_price; // 购买原价
    int price; // 购买价格
 
    // 指针域
    struct my_flight *next;
} st_myflg, *pt_myflg;

// 用户好友列表
typedef struct my_friend
{
    // 数据域
    char my_friend[32];

    // 指针域
    struct my_friend *next;
} st_friend, *pt_friend;

// 用户朋友圈
typedef struct user_twitter
{
    // 数据域
    char time[25];
    char text[300];

    // 指针域
    struct user_twitter *next;
} st_twitter, *pt_twitter;

// 用户信息
typedef struct user_info
{
    // 数据域
    // 套接字（初始值为-1）
    int connfd;
    // 聊天室状态（初始值为0）
    int istalk;
    // 身份证
    char id[ID_MAX_LEN + 1];
    // 是否为管理员
    int isroot;
    // 用户名
    char name[NAME_MAX_LEN + 1];
    // 密码
    char pwd[PWD_MAX_LEN + 1];
    // 手机号
    char tel[TEL_MAX_LEN + 1];
    // 朋友列表使用链表存取
    pt_friend myFriend;
    // 航班列表使用链表存取
    pt_myflg myflg;
    // 余额
    int cash;
    // 是否为vip
    int vip;
    // 朋友圈使用链表存取
    pt_twitter myTwitter;

    // 指针域
    struct list_head list; //（前后关系）

} st_uinfo, *pt_uinfo;

// 登录管理结构体
struct manager
{
    pt_uinfo head;
    int count;
};

// 存放全局函数
/************** my_socket **************/
// 初始化套接字
extern int init_sock(const char *port);
// 获取用户输入
extern void recv_fun(const char *tip, int connfd, char *buf);

/************** socket_file **************/
extern int send_view(int connfd, char *filename); //成功返回 0，失败返回 1

/************** socket_manager **************/
// 登录界面
extern void login_view_options(int connfd);
// 用户界面
extern void main_view_options(int connfd, pt_uinfo user);
// 管理员界面
extern void main_root_options(int connfd, pt_uinfo user);

/************** user_link **************/
// 初始化内核链表
extern pt_uinfo user_info_init(pt_uinfo head);
// 初始化登录管理结构体
extern int init_boss(struct manager *boss);
// 读取用户文件
extern void read_user_file();
// 从文件中添加用户数据
extern void add_user_info(char *text);
// 打印链表所有数据（后序遍历）
extern void user_info_list_show(pt_uinfo head);
// 添加用户数据到链表尾
extern int user_info_add_tail(cJSON *arrayItem, pt_uinfo head);
// 初始化朋友链表
extern pt_friend friend_init(pt_uinfo head);
// 添加好友列表
extern int add_friend_tail(pt_friend head, char *fri);
// 初始化航班列表
extern pt_myflg flight_init(pt_uinfo head);
// 添加航班列表
extern int add_flight_tail(pt_myflg head, cJSON *arrayItem);
// 初始化朋友圈列表
extern pt_twitter twitter_init(pt_uinfo head);
// 添加朋友圈列表
extern int add_twitter_tail(pt_twitter head, cJSON *arrayItem);

/************** flight_link **************/
// 初始化航班管理结构体
extern int init_plane(struct plane_manager *p_manager);
// 初始化内核链表
extern pt_flg plane_info_init(pt_flg head);
// 读取文件
extern char *read_plane_file(const char *filename);
// 从文件中添加航班数据
extern void add_plan_info(char *data);
// 文件添加用户数据到链表尾
extern int plane_info_add_tail(cJSON *arrayItem, pt_flg head);
// 打印链表所有数据（后序遍历）
extern void plane_info_list_show(pt_flg head);

/************** flight_view **************/
// 显示全部航班
extern void view_flight_list(pt_uinfo user);
// 价格补0
extern void cash_cat(int num, char *cost);
// 显示查询航班
extern int view_flight_search(pt_uinfo user, char *code, char *e_plane, char *date, char *type, int price);

/************** search_flight **************/
// 查询航班
extern void search_fligth_list(pt_uinfo user);
// 快速查询航班
extern void quick_search(pt_uinfo user);

/************** user_change **************/
// 入口函数
extern void user_change_main(pt_uinfo user);
// 修改手机号
extern void user_change_tel(pt_uinfo user);
// 修改密码
extern void user_change_pwd(pt_uinfo user);
// 修改身份证
extern void user_change_id(pt_uinfo user);

/************** login **************/
// 登录
extern pt_uinfo user_login(int connfd);
// 注册
extern void user_reg(int connfd);
extern pt_uinfo reg_init();
// 修改密码
extern void change_pwd(int connfd);
// 检查用户名
extern pt_uinfo check_user_list(char *input, char *type);
// 退出登录
extern void quit_user(int connfd);
// 服务器退出
extern void quit_server();
// 最大连接数限制
extern int max_links();

/************** socket_msg **************/
// 获取客户端选项
extern int num_msg(int connfd);
// 发送用户数据
extern void send_user();

/************** change_info **************/
// 重新写入用户数据到文件中
extern void add_info_file(pt_uinfo head);
// 重新写入航班数据到文件中
extern void add_flight_file(pt_flg head);

/************** user_cash **************/
// 查询余额
extern void search_cash(pt_uinfo user);
// 余额充值
extern void charge_cash(pt_uinfo user);

/************** search_friend **************/
// 显示好友列表
extern void friend_list(pt_uinfo user);
// 添加好友
extern void add_friend_list(pt_uinfo user);
// 删除好友
extern void del_friend_list(pt_uinfo user);

/************** friend_send **************/
// 聊天功能入口函数
extern void friend_send_main();
// 好友聊天
extern void friend_send(pt_uinfo user);
// 群发消息
extern int friend_msg_broadcast(pt_uinfo user, const char *msg);
// 私聊
int friend_msg_send(pt_uinfo user, const char *name, const char *msg);

/************** list_free **************/
// 释放用户管理结构体
extern void boss_list_free();
// 释放航班管理结构体
extern void plane_list_free();

/************** user_flight **************/
// 已购航班列表
extern void user_flight_list(pt_uinfo user);
// 购买航班
extern void user_flight_buy(pt_uinfo user);
// 购票
extern int buy_tickets(pt_uinfo temp, pt_flg pos, pt_uinfo user);
// 发送界面
extern void flight_manager(pt_uinfo user);
// 改签
extern void change_flight(pt_uinfo user);
// 退票
extern void user_plane_del(pt_uinfo user);
// 取票
extern void user_plane_get(pt_uinfo user);

/************** root_flight **************/
// 删除航班
extern void flight_del(pt_uinfo user);
// 添加航班
extern void flight_add(pt_uinfo user);
// 添加新航班
extern void add_new_plane(int connfd, char *code);
// 修改现有航班
extern void change_old_plane(int connfd, pt_flg pos);

#endif