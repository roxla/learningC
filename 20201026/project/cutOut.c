#include "my.h"

// 以某个字符串为分割符分割字符串，并返回最后一个字符串
char *cutOut(char str[], const char *delim, char **catalog)
{
    char *result = NULL;
    //char *strtok(char s[], const char *delim);
    result = strtok(str, delim);
    char msg[ROW][COL];
    int i = 0;
    for (i = 0; result != NULL; i++)
    {
        strncpy(msg[i], result, sizeof(msg[i]));
        //printf("msg[%d] = %s", i, msg[i]);
        //printf("\n");
        result = strtok(NULL, delim);
    }
    *catalog = msg[i - 1];
    //printf("catalog = %s\n", *catalog);
    return *catalog;
}
