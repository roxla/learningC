#include <stdio.h>
#include <string.h>

struct bookInfo
{
    int num;       // 序号
    char name[20]; // 书名
    int page;      // 页数
    float cash;    // 价格
};

void input_info(struct bookInfo *info);
void show_info(struct bookInfo *info);

int main(void)
{
    int n, loop = 1;
    struct bookInfo stu1;
    bzero(&stu1, sizeof(struct bookInfo));
    while (loop)
    {
        printf("============TITLE=============\n");
        printf("1 写入数据到该结构体变量中  \
                2 显示结构体变量所有信息    \
                其他 退出\n\n");
        printf("请输入要执行的操作的序号: ");
        scanf("%d", &n);
        while (getchar() != '\n')
            ;
        switch (n)
        {
        case 1:
            input_info(&stu1);
            break;

        case 2:
            show_info(&stu1);
            break;
        default:
            loop = 0;
            break;
        }
    }

    return 0;
}

void input_info(struct bookInfo *info)
{
    printf("请输入书本的序号: ");
    scanf("%d", &(info->num));
    printf("请输入书本的书名: ");
    scanf("%s", info->name);
    printf("请输入书本的页数: ");
    scanf("%d", &(info->page));
    printf("请输入书本的价格: ");
    scanf("%f", &(info->cash));
}

void show_info(struct bookInfo *info)
{
    printf("============INFO=============\n");
    printf("序号: %d\n", info->num);
    printf("书名: %s\n", info->name);
    printf("页数: %d\n", info->page);
    printf("价格: %.2f\n", info->cash);
}