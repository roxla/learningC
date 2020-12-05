#include "head.h"

// 初始化航班管理结构体
int init_plane(struct plane_manager *p_manager)
{
    char filename[] = "/mnt/hgfs/roxla/C/20201124/server/file/data/flight";
    char *data;
    // 初始化头节点
    p_manager->head = plane_info_init(p_manager->head);
    p_manager->count = 0; //当前没有人连接进来
    // 打开文件，将用户信息存入链表
    // 读取文件
    data = read_plane_file(filename);
    add_plan_info(data);

    return 0;
}

// 初始化内核链表
pt_flg plane_info_init(pt_flg head)
{
    // 申请一块堆空间给头节点
    head = malloc(sizeof(st_flg));
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

// 读取文件
char *read_plane_file(const char *filename)
{
    FILE *f;
    long len;
    char *data;

    // 获取json文件内容长度
    f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    // 读取json文件
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);

    return data;
}

// 从文件中添加航班数据
void add_plan_info(char *data)
{
    cJSON *json, *object, *arrayItem;
    int size, i;
    // 解析JSON数据
    json = cJSON_Parse(data);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return;
    }
    // 可从cJSON结构体中查找某个子节点名称（键名称），如果查找成功可把该子节点序列化到cJSON结构体中
    object = cJSON_GetObjectItem(json, "flight");
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
        plane_info_add_tail(arrayItem, p_manager->head);
        p_manager->count++;
    }
    // 释放解析JSON数据时生成的空间
    cJSON_Delete(json);
    free(data);

    // 打印看看
    // plane_info_list_show(p_manager->head);
}

// 文件添加用户数据到链表尾
int plane_info_add_tail(cJSON *arrayItem, pt_flg head)
{
    cJSON *item;
    // 申请堆空间给新节点
    pt_flg new_node = malloc(sizeof(st_flg));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return 1;
    }
    bzero(new_node, sizeof(st_flg));
    // 将数据给入新节点的数据域
    // 航班号 唯一
    item = cJSON_GetObjectItem(arrayItem, "code");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->code, item->valuestring, strlen(item->valuestring));
    }
    // 余票
    item = cJSON_GetObjectItem(arrayItem, "num");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->num = item->valueint;
    }
    // 金额
    item = cJSON_GetObjectItem(arrayItem, "cash");
    if (item != NULL)
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valueint=%d\n", item->type, item->string, item->valueint);
        new_node->cash = item->valueint;
    }
    // 飞机类型
    item = cJSON_GetObjectItem(arrayItem, "type");
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->type, item->valuestring, strlen(item->valuestring));
    }
    // 起飞时间
    item = cJSON_GetObjectItem(arrayItem, "time");
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->time, item->valuestring, strlen(item->valuestring));
    }
    // 班期
    item = cJSON_GetObjectItem(arrayItem, "date");
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->date, item->valuestring, strlen(item->valuestring));
    }
    // 起点站
    item = cJSON_GetObjectItem(arrayItem, "s_plane");
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->s_plane, item->valuestring, strlen(item->valuestring));
    }
    // 终点站
    item = cJSON_GetObjectItem(arrayItem, "e_plane");
    {
        // printf("cJSON_GetObjectItem: type=%d, string is %s, valuestring=%s\n", item->type, item->string, item->valuestring);
        memcpy(new_node->e_plane, item->valuestring, strlen(item->valuestring));
    }
    // 修改指向
    list_add_tail(&new_node->list, &head->list);

    return 0;
}

// 打印链表所有数据（后序遍历）
void plane_info_list_show(pt_flg head)
{
    pt_flg get_node, n;
    // 从第一个节点遍历所有的节点，直到head
    printf("==========后序===========\n");
    list_for_each_entry_safe(get_node, n, &head->list, list)
    {
        // 航班号 唯一
        printf("code: %s\n", get_node->code);
        // 余票
        printf("num: %d\n", get_node->num);
        // 金额
        printf("cash: %d\n", get_node->cash);
        // 飞机类型
        printf("type: %s\n", get_node->type);
        // 起飞时间
        printf("time: %s\n", get_node->time);
        // 班期
        printf("date: %s\n", get_node->date);
        // 起点站
        printf("s_plan: %s\n", get_node->s_plane);
        // 终点站
        printf("e_plan: %s\n", get_node->e_plane);
        // 分割
        printf("\n");
    }
}
