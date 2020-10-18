#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define ROW 10
#define COL 25

// 以某个字符串为分割符分割字符串，并返回最后一个字符串
char *cutOut(char str[], const char *delim, char **catalog); // 参数 要分割的字符串 分割符 返回的指针的地址

int main(int argc, char const *argv[])
{
    // if (argc != 3)
    // {
    //     // 输入参数个数不为2
    //     printf("Input error!\n");
    //     return 1;
    // }

    char name[256], _name[15] = "ebm";
    char *catalog;
    bool y = false, n = false;
    // strncpy(_name, argv[2], sizeof(_name));
    DIR *dp = NULL;
    // dp = opendir(argv[1]);
    dp = opendir("test1");

    if (dp == NULL)
    {
        perror("stat failed");
        return 1;
    }

    struct dirent *ep = NULL;
    while (1)
    {
        bzero(name, sizeof(name));
        ep = readdir(dp);
        if (ep == NULL)
            break;
        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;
        strncpy(name, ep->d_name, sizeof(name));

        cutOut(name, ".", &catalog);

        if (!strcmp(catalog, _name))
        {
            printf("%s\n", ep->d_name);
            y = true;
        }
        // printf("%d\n", length);
        // printf("%s\n", name);
    }
    if (!(y || n))
    {
        printf("File not found\n");
    };

    closedir(dp);
    return 0;
}

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
