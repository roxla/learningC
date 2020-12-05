// #include "head.h"

// void doit(char *text)
// {
//     char *out;
//     cJSON *json;

//     json = cJSON_Parse(text);
//     if (!json)
//     {
//         printf("Error before: [%s]\n", cJSON_GetErrorPtr());
//     }
//     else
//     {
//         out = cJSON_Print(json);
//         cJSON_Delete(json);
//         printf("%s\n", out);
//         free(out);
//     }
// }

// void dofile(char *filename)
// {
//     FILE *f;
//     long len;
//     char *data;

//     // 打开文件
//     f = fopen(filename, "rb");
//     // 获取文件总长度
//     fseek(f, 0, SEEK_END);
//     len = ftell(f);
//     //读取文件
//     fseek(f, 0, SEEK_SET);
//     data = (char *)malloc(len + 1);
//     fread(data, 1, len, f);

//     fclose(f);
//     // 写入文件数据
//     doit(data);

//     free(data);
// }

// void read_data(char *filename)
// {
//     // 入口函数
//     dofile(filename);
// }