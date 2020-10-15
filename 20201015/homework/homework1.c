#include <errno.h>
#include <fcntl.h>
#include <linux/input.h> //输入子系统头文件
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 800 * 480

/*************** 全局变量 ***************/
int lcd_fd;       // LCD文件描述符
unsigned int *FB; // 显存首地址

int ts_fd; //触摸屏文件描述符

/*************** LCD操作函数声明 ***************/
// 打开LCD
int open_lcd(void);
// 关闭LCD
void close_lcd(void);
// 显示全屏ebm图片
int show_pic_full(char *pathname);

/*************** 触摸屏操作函数声明 ***************/
// 获取xy坐标(通过地址传递，修改两个变量数据)
void get_xy(int *x, int *y);
// 打开触摸屏
int open_ts(void);
// 关闭触摸屏
void close_ts(void);

/*************** 主函数 ***************/
int main(void)
{
    /****** 1.打开设备 *****/
    open_lcd();
    open_ts();

    /****** 2.程序逻辑代码 *****/
    int pos_x, pos_y, length, i = -1;
    char pic_arr[][10] = {"1.ebm", "2.ebm", "3.ebm", "4.ebm", "5.ebm", "6.ebm", "7.ebm"};
    length = sizeof(pic_arr) / sizeof(pic_arr[0]); // 计算数组内元素个数
    while (1)
    {
        get_xy(&pos_x, &pos_y);
        printf("(%d, %d)\n", pos_x, pos_y);

        if (pos_x >= 0 && pos_x < 400)
        {
            printf("Left!\n");
            i -= 1;
            if (i <= -1)
                i = length - 1;
            // printf("%d\n", i);
            show_pic_full(pic_arr[i]);
        }
        else if (pos_x >= 400 && pos_x <= 800)
        {
            printf("Right!\n");
            i += 1;
            if (i >= length)
                i = 0;
            // printf("%d\n", i);
            show_pic_full(pic_arr[i]);
        }
    }

    /****** 3.关闭设备 *****/
    close_ts();
    close_lcd();

    return 0;
}

/*************** 子函数源码 ***************/
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

        // 坐标范围处理 (0-1024, 0-600) --> (0-800, 0-480)
        // x1 = ((float)x/1024)*800;
        // y1 = ((float)y/600)*480;
        *x = *x * 800 / 1024;
        *y = *y * 480 / 600;

        // 当获取到了x和y坐标
        if (x_ready && y_ready)
            break;
    }
}

int show_pic_full(char *pathname)
{
    // 拼接图片路径
    char pic_name[20] = "pic/";
    strncat(pic_name, pathname, sizeof(pic_name) - strlen(pathname) - 1);
    // printf("%s\n", pic_name);
    // 打开ebm图片文件
    int ebm_fd;
    ebm_fd = open(pic_name, O_RDONLY);
    if (ebm_fd == -1)
    {
        perror("open ebm failed");
        return 1;
    }

    // 读取aRGB像素数据
    unsigned int ebm_buf[800 * 480] = {0};
    read(ebm_fd, ebm_buf, sizeof(ebm_buf));

    // 关闭ebm文件描述符
    close(ebm_fd);

    // 将每一个像素写入到LCD显存中
    int x, y;
    for (y = 0; y < 480; y++)
        for (x = 0; x < 800; x++)
            *(FB + x + 800 * y) = ebm_buf[x + 800 * y];

    return 0;
}

int open_lcd(void)
{

    lcd_fd = open("/dev/fb0", O_RDWR); //打开LCD文件
    if (lcd_fd == -1)                  //判断lcd_fd是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    //映射显存
    FB = mmap(NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
              SIZE * 4,               // 内存的大小（开发板屏幕4*800*480字节）
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
    munmap(FB, SIZE * 4); //取消映射
    close(lcd_fd);        //关闭文件
}
