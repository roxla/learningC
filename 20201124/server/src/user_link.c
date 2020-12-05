#include "head.h"

// 初始化内核链表
pt_uinfo user_info_init(pt_uinfo head)
{
    // 申请一块堆空间给头节点
    head = malloc(sizeof(st_uinfo));
    if (head == NULL)
    {
        perror("head malloc failed");
        return NULL;
    }

    // 修改头节点的指针域，使它们都指向自身
    INIT_LIST_HEAD(&head->list);

    // 将堆空间首地址返回
    return head;
}

// 初始化朋友链表
pt_friend friend_init(pt_uinfo head)
{
    // 给头节点申请空间
    head->myFriend = malloc(sizeof(st_friend));
    if (head->myFriend == NULL)
    {
        perror("myFriend malloc failed");
        return NULL;
    }
    // 让头节点的指针域next指向NULL
    head->myFriend->next = NULL;

    // 将堆空间首地址返回
    return head->myFriend;
}

// 添加好友列表
int add_friend_tail(pt_friend head, char *fri)
{
    // 新节点分配堆空间
    pt_friend new_node = malloc(sizeof(st_friend));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }
    // 把数据给入新节点数据域
    memcpy(new_node->my_friend, fri, strlen(fri));
    // 让新节点指向空
    new_node->next = NULL;
    // 找出尾节点（遍历链表，找出指针域指向NULL的节点）
    while (head->next)
        head = head->next;

    // 修改指向
    head->next = new_node;
}

// 初始化航班列表
pt_myflg flight_init(pt_uinfo head)
{
    // 给头节点申请空间
    head->myflg = malloc(sizeof(st_myflg));
    if (head->myFriend == NULL)
    {
        perror("myflg malloc failed");
        return NULL;
    }
    // 让头节点的指针域next指向NULL
    head->myflg->next = NULL;

    // 将堆空间首地址返回
    return head->myflg;
}

// 添加航班列表
int add_flight_tail(pt_myflg head, cJSON *arrayItem)
{
    cJSON *item;
    // 新节点分配堆空间
    pt_myflg new_node = malloc(sizeof(st_myflg));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }
    // 把数据给入新节点数据域
    item = cJSON_GetObjectItem(arrayItem, "plane");
    if (item != NULL)
    {
        printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->flight, item->valuestring, strlen(item->valuestring));
    }
    item = cJSON_GetObjectItem(arrayItem, "o_cost");
    if (item != NULL)
    {
        printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->o_price = item->valueint;
    }
    item = cJSON_GetObjectItem(arrayItem, "cost");
    if (item != NULL)
    {
        printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->price = item->valueint;
    }
    // 让新节点指向空
    new_node->next = NULL;
    // 找出尾节点（遍历链表，找出指针域指向NULL的节点）
    while (head->next)
        head = head->next;

    // 修改指向
    head->next = new_node;
}

// 初始化朋友圈列表
pt_twitter twitter_init(pt_uinfo head)
{
    // 给头节点申请空间
    head->myTwitter = malloc(sizeof(st_twitter));
    if (head->myTwitter == NULL)
    {
        perror("myTwitter malloc failed");
        return NULL;
    }
    // 让头节点的指针域next指向NULL
    head->myTwitter->next = NULL;

    // 将堆空间首地址返回
    return head->myTwitter;
}

// 添加朋友圈列表
int add_twitter_tail(pt_twitter head, cJSON *arrayItem)
{
    cJSON *item;
    // 新节点分配堆空间
    pt_twitter new_node = malloc(sizeof(st_twitter));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }
    // 把数据给入新节点数据域
    // 添加时间
    item = cJSON_GetObjectItem(arrayItem, "time");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->time, item->valuestring, strlen(item->valuestring));
    }
    // 添加内容
    item = cJSON_GetObjectItem(arrayItem, "text");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->text, item->valuestring, strlen(item->valuestring));
    }

    // 让新节点指向空
    new_node->next = NULL;
    // 找出尾节点（遍历链表，找出指针域指向NULL的节点）
    while (head->next)
        head = head->next;

    // 修改指向
    head->next = new_node;
}

// 文件添加用户数据到链表尾
int user_info_add_tail(cJSON *arrayItem, pt_uinfo head)
{
    cJSON *item, *arrItem, *cItem;
    int i;
    // 申请堆空间给新节点
    pt_uinfo new_node = malloc(sizeof(st_uinfo));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }
    bzero(new_node, sizeof(st_uinfo));
    // 将数据给入新节点的数据域
    new_node->connfd = -1;
    new_node->istalk = 0;
    // 添加是否为管理员
    item = cJSON_GetObjectItem(arrayItem, "isroot");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->isroot = item->valueint;
    }
    // 添加身份证
    item = cJSON_GetObjectItem(arrayItem, "id");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->id, item->valuestring, strlen(item->valuestring));
    }
    // 添加用户名
    item = cJSON_GetObjectItem(arrayItem, "name");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->name, item->valuestring, strlen(item->valuestring));
    }
    // 添加密码
    item = cJSON_GetObjectItem(arrayItem, "pwd");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->pwd, item->valuestring, strlen(item->valuestring));
    }
    // 添加手机号
    item = cJSON_GetObjectItem(arrayItem, "tel");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->tel, item->valuestring, strlen(item->valuestring));
    }
    // 初始化朋友链表
    new_node->myFriend = friend_init(new_node);
    // 添加朋友列表
    item = cJSON_GetObjectItem(arrayItem, "friend");
    if (arrayItem != NULL)
    {
        int size = cJSON_GetArraySize(item);
        for (i = 0; i < size; i++)
        {
            arrItem = cJSON_GetArrayItem(item, i);
            // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", arrItem->type, arrItem->string, arrItem->valuestring);
            add_friend_tail(new_node->myFriend, arrItem->valuestring);
        }
    }
    // 初始化航班列表
    new_node->myflg = flight_init(new_node);
    // 添加航班列表
    item = cJSON_GetObjectItem(arrayItem, "flight");
    if (arrayItem != NULL)
    {
        int size = cJSON_GetArraySize(item);
        for (i = 0; i < size; i++)
        {
            arrItem = cJSON_GetArrayItem(item, i);
            printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", arrItem->type, arrItem->string, arrItem->valuestring);
            add_flight_tail(new_node->myflg, arrItem);
        }
    }
    // 添加余额
    item = cJSON_GetObjectItem(arrayItem, "cash");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->cash = item->valueint;
    }
    // 添加是否为vip
    item = cJSON_GetObjectItem(arrayItem, "vip");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->vip = item->valueint;
    }
    // 初始化朋友圈
    new_node->myTwitter = twitter_init(new_node);
    // 添加朋友圈
    item = cJSON_GetObjectItem(arrayItem, "twitter");
    if (arrayItem != NULL)
    {
        int size = cJSON_GetArraySize(item);
        for (i = 0; i < size; i++)
        {
            arrItem = cJSON_GetArrayItem(item, i);
            printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", arrItem->type, arrItem->string, arrItem->valuestring);
            add_twitter_tail(new_node->myTwitter, arrItem);
        }
    }

    // 修改指向
    list_add_tail(&new_node->list, &head->list);

    return 0;
}

// 初始化登录管理结构体
int init_boss(struct manager *boss)
{
    // 初始化头节点
    boss->head = user_info_init(boss->head);
    boss->count = 0; //当前没有人连接进来
    // 打开文件，将用户信息存入链表
    read_user_file();

    return 0;
}

// 读取用户文件
void read_user_file()
{
    FILE *f;
    long len;
    char *data;
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/user";

    // 获取json文件内容长度
    f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    // 读取json文件
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);
    add_user_info(data);

    // 释放空间
    free(data);
}

// 从文件中添加用户数据
void add_user_info(char *text)
{
    cJSON *json, *object, *arrayItem;
    int size, i;
    // 解析JSON数据
    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return;
    }
    // 可从cJSON结构体中查找某个子节点名称（键名称），如果查找成功可把该子节点序列化到cJSON结构体中
    object = cJSON_GetObjectItem(json, "user");
    if (!object)
    {
        printf("Json object error\n");
        return;
    }
    // 获取数组中对象的个数
    size = cJSON_GetArraySize(object);
    // printf("cJSON_GetArraySize: size=%d\n", size);
    // 循环数组
    for (i = 0; i < size; i++)
    {
        // printf("i=%d\n", i);
        arrayItem = cJSON_GetArrayItem(object, i);
        user_info_add_tail(arrayItem, boss->head);
    }
    // 释放解析JSON数据时生成的空间
    cJSON_Delete(json);

    // 打印看看
    // user_info_list_show(boss->head);
}

// 打印链表所有数据（后序遍历）
void user_info_list_show(pt_uinfo head)
{
    pt_uinfo get_node, n;
    pt_friend fri_pos;
    pt_myflg flg_pos;
    pt_twitter twi_pos;
    // 从第一个节点遍历所有的节点，直到head
    printf("==========后序===========\n");
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        // 套接字
        printf("connfd: %d\n", get_node->connfd);
        // id
        printf("id: %s\n", get_node->id);
        // 是否为管理
        printf("isroot: %d\n", get_node->isroot);
        // 用户名
        printf("name: %s\n", get_node->name);
        // 密码
        printf("pwd: %s\n", get_node->pwd);
        // 电话
        printf("tel: %s\n", get_node->tel);
        // 好友列表
        for (fri_pos = get_node->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
            printf("my_friend: %s\n", fri_pos->my_friend);
        // 航班列表
        for (flg_pos = get_node->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
        {
            printf("my_flight: %s", flg_pos->flight);
            printf(" %d\n", flg_pos->price);
        }
        // 余额
        printf("cash: %d\n", get_node->cash);
        // 是否为vip
        printf("vip: %d\n", get_node->vip);
        // 朋友圈
        for (twi_pos = get_node->myTwitter->next; twi_pos != NULL; twi_pos = twi_pos->next)
        {
            printf("========== %s =========\n", twi_pos->time);
            printf("my_twitter: %s\n", twi_pos->text);
            printf("=====================\n");
        }
        // 分割
        printf("\n");
    }
}