#include "head.h"

int send_view(int connfd, char *filename)
{
    // 打开文件
    FILE *fp;
    long len;
    char *data;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        sleep(1);
        printf("fopen failed\n");
        return 1;
    }
    // 读取文件长度
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    // 设置文件偏移到开头
    fseek(fp, 0, SEEK_SET);
    // 读取文件
    data = (char *)malloc(len + 1);
    fread(data, 1, len, fp);
    // 文件读取错误时返回
    if (ferror(fp))
    {
        sleep(1);
        printf("Error to read the file\n");
        return 1;
    }
    // 关闭文件
    fclose(fp);

    // 发送读取到的内容
    send(connfd, data, len, 0);

    // 清空空间
    free(data);

    return 0;
}