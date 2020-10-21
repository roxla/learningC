#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h> //输入子系统头文件
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ROW 10
#define COL 25
#define PIC_TYPE "ebm" // 图片文件后缀
#define PIC_NUM 20     // 图片列表长度
#define SIZE_W 800     //屏幕高度
#define SIZE_H 480     //屏幕高度
#define PIC_PATH "pic" //图片存储目录

/*************** 全局变量 ***************/
int lcd_fd;       // LCD文件描述符
unsigned int *FB; // 显存首地址
int ts_fd;        //触摸屏文件描述符

/*************** LCD操作函数声明 ***************/
// 打开LCD
int open_lcd(void);
// 关闭LCD
void close_lcd(void);
// 显示全屏ebm图片
int show_pic_full(const char *pathname);

/*************** 触摸屏操作函数声明 ***************/
// 获取xy坐标(通过地址传递，修改两个变量数据)
void get_xy(int *x, int *y);
// 打开触摸屏
int open_ts(void);
// 关闭触摸屏
void close_ts(void);

/*************** 目录操作函数声明 ***************/
// 打开目录文件并获取图片文件
int get_pic_list(char (*list)[strlen(PIC_PATH) + 257]);
// 以某个字符串为分割符分割字符串，并返回最后一个字符串
char *cutOut(char str[], const char *delim, char **catalog); // 参数 要分割的字符串 分割符 返回的指针的地址

/*************** 主函数 ***************/
int main(int argc, char const *argv[])
{
    /*************** 变量定义 ***************/
    int i = -1;
    int pos_x, pos_y;
    int length;

    char(*pic_list)[strlen(PIC_PATH) + 257] = calloc(PIC_NUM, strlen(PIC_PATH) + 257); // 直接使用ep->d_name进行拼接需要的空间为 256 + 1 (最后一位用于存储'\0')
    if (pic_list == NULL)
    {
        perror("callo failed");
        return 1;
    }
    bzero(pic_list, sizeof(pic_list[0]));

    /****** 1.打开设备 *****/
    open_lcd();
    open_ts();

    /****** 2.获取图片列表 *****/
    length = get_pic_list(pic_list);
    printf("%d\n", length);
    /****** 3.程序逻辑代码 *****/
    if (length) // 实际存入的图片数量不为0时执行显示图片
    {
        while (1)
        {
            get_xy(&pos_x, &pos_y); // 获取xy坐标
            if (pos_x >= 0 && pos_x < 400)
            {
                printf("Left!\n");
                i--;
                if (i < 0)
                    i = 4;

                printf("picname: %s\n", pic_list[i]);
                show_pic_full(pic_list[i]);
            }
            else if (pos_x >= 400 && pos_x <= SIZE_W)
            {
                printf("Right!\n");
                i++; // 0 1 2 3 4
                if (i >= length)
                    i = 0;

                printf("picname: %s\n", pic_list[i]);
                show_pic_full(pic_list[i]);
            }
        }
    }

    /****** 4.关闭设备 *****/
    close_ts();
    close_lcd();
    /***** 5.释放堆空间 *****/
    free(pic_list);

    return 0;
}

// 打开目录文件并获取图片文件
int get_pic_list(char (*list)[strlen(PIC_PATH) + 257])
{
    /*************** 变量定义 ***************/
    int i = 0;
    char name[256];
    char *catalog;
    bool pic_file = false;

    /*************** 目录操作函数 ***************/

    DIR *dp = NULL;
    dp = opendir(PIC_PATH);
    if (dp == NULL)
    {
        perror("stat failed");
        return 0;
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
        // 以“.”为基准分割字符串并返回最后一个字符串
        cutOut(name, ".", &catalog);

        if (!strcmp(catalog, PIC_TYPE))
        {
            if (i >= PIC_NUM)
                break;
            snprintf(list[i], sizeof(list[i]), "%s/%s", PIC_PATH, ep->d_name);
            // printf("%s\n", ep->d_name);
            pic_file = true;
            i++;
        }
    }

    if (!pic_file)
    {
        printf("File not found\n");
    };

    /*************** 关闭文件夹  ***************/
    closedir(dp);

    return i;
}

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

// 打开触摸屏
int open_ts(void)
{
    // 1.打开触摸屏设备文件	--->   “/dev/input/event0”
    ts_fd = open("/dev/input/event0", O_RDONLY);
    if (ts_fd == -1)
    {
        perror("open ts failed");
        return 1;
    }

    return 0;
}

// 关闭触摸屏
void close_ts(void)
{
    // 3.关闭触摸屏设备文件
    close(ts_fd);
}

// 获取xy坐标(通过地址传递，修改两个变量数据)
void get_xy(int *x, int *y)
{
    // 2.读取触摸屏数据（需要使用输入子系统对原始数据进行解析）
    struct input_event ts_buf;
    bool x_ready = false, y_ready = false;
    int pres = -1;

    while (1)
    {
        bzero(&ts_buf, sizeof(ts_buf));
        read(ts_fd, &ts_buf, sizeof(ts_buf));
        //printf("type:0x%x code:0x%x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);
        // if(ts_buf.type == 0x3)
        if (ts_buf.type == EV_ABS) // 触摸屏事件
        {
            if (ts_buf.code == ABS_X) //X轴
            {
                *x = ts_buf.value;
                x_ready = true;
                y_ready = false; //确保先获取到的是x坐标
            }
            else if (ts_buf.code == ABS_Y) //Y轴
            {
                *y = ts_buf.value;
                y_ready = true;
            }
        }

        if (ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH)
            pres = ts_buf.value;

        // 坐标范围处理 (0-1024, 0-600) --> (0-800, 0-480)
        // x1 = ((float)x/1024)*800;
        // y1 = ((float)y/600)*480;

        // 当获取到了x和y坐标，并且松开
        if (x_ready && y_ready && pres == 0)
        {
            *x = *x * SIZE_W / 1024;
            *y = *y * SIZE_H / 600;
            break;
        }
    }
}

// 显示全屏ebm图片
int show_pic_full(const char *pathname)
{
    // 打开ebm图片文件
    int ebm_fd;
    ebm_fd = open(pathname, O_RDONLY);
    if (ebm_fd == -1)
    {
        perror("open ebm failed");
        return 1;
    }

    // 读取aRGB像素数据
    unsigned int ebm_buf[SIZE_W * SIZE_H] = {0};
    read(ebm_fd, ebm_buf, sizeof(ebm_buf));

    // 关闭ebm文件描述符
    close(ebm_fd);

    // 将每一个像素写入到LCD显存中
    int x, y;
    for (y = 0; y < SIZE_H; y++)
        for (x = 0; x < SIZE_W; x++)
            *(FB + x + SIZE_W * y) = ebm_buf[x + SIZE_W * y];

    return 0;
}

// 打开LCD
int open_lcd(void)
{

    lcd_fd = open("/dev/fb0", O_RDWR); //打开LCD文件
    if (lcd_fd == -1)                  //判断lcd_fd是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    //映射显存
    FB = mmap(
        NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
        SIZE_H * SIZE_W * 4,    // 内存的大小（开发板屏幕4*800*480字节）
        PROT_READ | PROT_WRITE, // 保护权限（PROT_READ|PROT_WRITE）
        MAP_SHARED,             // 共享的	MAP_SHARED
        lcd_fd,                 // 映射的文件描述符
        0);                     // 映射后偏移量（一般填0，不偏移）
    if (FB == MAP_FAILED)
    {
        perror("mmap failed");
        return 2;
    }

    return 0;
}

// 关闭lcd
void close_lcd(void)
{
    // 3.取消映射并关闭LCD
    munmap(FB, SIZE_W * SIZE_H * 4); //取消映射
    close(lcd_fd);                   //关闭文件
}
