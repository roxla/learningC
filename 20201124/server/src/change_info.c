#include "head.h"

// 重新写入用户数据到文件中
void add_info_file(pt_uinfo head)
{
    // 定义变量
    long len;
    
    // 头
    cJSON *root = cJSON_CreateObject();
    // 头下面的对象
    pt_uinfo get_node, n;
    cJSON *userArray = cJSON_CreateArray();
    cJSON *item;
    // 对象中的朋友列表
    pt_friend fri_pos;
    cJSON *frItem; //嵌套的创建对象需要循环
    // 对象中的航班列表
    pt_myflg flg_pos;
    cJSON *flArray; //嵌套的创建对象需要循环
    cJSON *flItem; //嵌套的创建对象需要循环
    // 对象中的朋友圈列表
    pt_twitter twi_pos;
    cJSON *twArray; //嵌套的创建对象需要循环
    cJSON *twItem;

    // 写入数据到cJson中
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        // 创建一个对象
        item = cJSON_CreateObject();
        // 是否为管理
        cJSON_AddNumberToObject(item, "isroot", get_node->isroot);
        // 身份证
        cJSON_AddStringToObject(item, "id", get_node->id);
        // 用户名
        cJSON_AddStringToObject(item, "name", get_node->name);
        // 密码
        cJSON_AddStringToObject(item, "pwd", get_node->pwd);
        // 电话
        cJSON_AddStringToObject(item, "tel", get_node->tel);
        // 朋友列表
        cJSON_AddItemToObject(item, "friend", frItem = cJSON_CreateArray()); // 创建一个数组
        for (fri_pos = get_node->myFriend->next; fri_pos != NULL; fri_pos = fri_pos->next)
        {
            // 向数组中添加没有key的对象
            cJSON_AddItemToArray(frItem, cJSON_CreateString(fri_pos->my_friend));
        }
        // 航班
        cJSON_AddItemToObject(item, "flight", flArray = cJSON_CreateArray()); // 创建一个数组
        for (flg_pos = get_node->myflg->next; flg_pos != NULL; flg_pos = flg_pos->next)
        {
            // 装入一个对象
            flItem = cJSON_CreateObject();
            // 向数组中添加没有key的对象
            cJSON_AddStringToObject(flItem, "plane", flg_pos->flight);
            cJSON_AddNumberToObject(flItem, "o_cost", flg_pos->o_price);
            cJSON_AddNumberToObject(flItem, "cost", flg_pos->price);
            // 将对象与装入数组(向数组中增加对象)
            cJSON_AddItemToArray(flArray, flItem);
        }
        // 余额
        cJSON_AddNumberToObject(item, "cash", get_node->cash);
        // vip
        cJSON_AddNumberToObject(item, "vip", get_node->vip);
        // 朋友圈
        cJSON_AddItemToObject(item, "twitter", twArray = cJSON_CreateArray()); // 创建一个数组
        for (twi_pos = get_node->myTwitter->next; twi_pos != NULL; twi_pos = twi_pos->next)
        {
            // 装入一个对象
            twItem = cJSON_CreateObject();
            // 对象中装入数据
            cJSON_AddStringToObject(twItem, "time", twi_pos->time);
            cJSON_AddStringToObject(twItem, "text", twi_pos->text);
            // 将对象与装入数组(向数组中增加对象)
            cJSON_AddItemToArray(twArray, twItem);
        }
        // 将对象与装入数组(向数组中增加对象)
        cJSON_AddItemToArray(userArray, item);
    }
    cJSON_AddItemToObject(root, "user", userArray); // 将数组装入对象(向对象中增加数组)
    // 新建一个user_bak文件
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/user_bak";
    FILE *fp = fopen(filename, "w+");

    //将传入的JSON结构转化为字符串
    char *buf = cJSON_Print(root);

    // 将数据写入到文件中
    len = fwrite(buf, strlen(buf), 1, fp);
    fclose(fp); //关闭文件
    free(buf);  //释放资源

    char oldname[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/user";
    if (len == 1)
    {
        // 写入成功，删除user文件，并将user_bak文件名改为user
        remove(oldname);
        rename(filename, oldname);
        printf("write success!\n");
    }
    else
    {
        // 写入失败，删除user_bak文件
        remove(filename);
        printf("write error!\n");
    }
}

// 重新写入航班数据到文件中
void add_flight_file(pt_flg head)
{
    // 定义变量
    long len;

    // 头
    cJSON *root = cJSON_CreateObject();
    // 头下面的对象
    pt_flg get_node, n;
    cJSON *userArray = cJSON_CreateArray();
    cJSON *item;
    // 写入数据到cJson中
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        // 创建一个对象
        item = cJSON_CreateObject();
        // 航班号
        cJSON_AddStringToObject(item, "code", get_node->code);
        // 金额
        cJSON_AddNumberToObject(item, "cash", get_node->cash);
        // 余票
        cJSON_AddNumberToObject(item, "num", get_node->num);
        // 起点站
        cJSON_AddStringToObject(item, "s_plane", get_node->s_plane);
        // 终点站
        cJSON_AddStringToObject(item, "e_plane", get_node->e_plane);
        // 飞机类型
        cJSON_AddStringToObject(item, "type", get_node->type);
        // 起飞时间
        cJSON_AddStringToObject(item, "time", get_node->time);
        // 班期
        cJSON_AddStringToObject(item, "date", get_node->date);
        // 将对象与装入数组(向数组中增加对象)
        cJSON_AddItemToArray(userArray, item);
    }
    cJSON_AddItemToObject(root, "flight", userArray); // 将数组装入对象(向对象中增加数组)

    // 新建一个user_bak文件
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/flight_bak";
    FILE *fp = fopen(filename, "w+");

    //将传入的JSON结构转化为字符串
    char *buf = cJSON_Print(root);

    // 将数据写入到文件中
    len = fwrite(buf, strlen(buf), 1, fp);
    fclose(fp); //关闭文件
    free(buf);  //释放资源

    char oldname[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/flight";
    if (len == 1)
    {
        // 写入成功，删除user文件，并将user_bak文件名改为user
        remove(oldname);
        rename(filename, oldname);
        printf("write success!\n");
    }
    else
    {
        // 写入失败，删除user_bak文件
        remove(filename);
        printf("write error!\n");
    }
}