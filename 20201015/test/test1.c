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

// 全局变量
int ts_fd;        //触摸屏文件描述符
int lcd_fd;       // lcd文件全局变量
unsigned int *FB; // 映射内存地址全局变量

// 获取xy坐标(通过地址传递，修改两个变量数据)
void get_xy(int *x, int *y);
// 打开lcd
int open_lcd(void);
// 打开触摸屏设备文件
int open_ts();
// 显示颜色
void show_color(int x_start, int y_start, int w, int h, unsigned int color); // 参数 X轴起始位置 Y轴起始位置 色块宽度 色块高度 颜色
// 全屏显示图片
int show_pic_full(const char *fd_name); // 参数 图片名字

int main(int argc, char const *argv[])
{
    int lcd = 0, ts = 0;

    lcd = open_lcd();
    if (lcd != 0)
        return 0;
    ts = open_ts();
    if (ts != 0)
        return 0;

    int pos_x, pos_y;
    while (1)
    {
        get_xy(&pos_x, &pos_y);
        if (pos_x < 400)
        {
            show_color(0, 0, 800, 480, 0x00FFFFFF);
        }
        else
        {
            show_pic_full("1.ebm");
        }
    }

    // 3.取消映射并关闭LCD
    munmap(FB, 800 * 480 * 4); // 取消映射
    close(lcd_fd);             // 关闭文件
    close(ts_fd);              // 关闭触摸屏设备文件

    return 0;
}

// 打开触摸屏设备文件
int open_ts()
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

// 打开lcd
int open_lcd(void)
{
    // 1.打开并映射
    lcd_fd = open("/dev/fb0", O_RDWR); //打开LCD文件
    if (lcd_fd == -1)                  //判断lcd_fd是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    //映射显存
    FB = mmap(
        NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
        800 * 480 * 4,          // 内存的大小（开发板屏幕4*800*480字节）
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

int show_pic_full(const char *fd_name)
{
    int img_fd;
    img_fd = open(fd_name, O_RDONLY);
    if (img_fd == -1) //判断img_fd_1是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    // 读取图片
    unsigned int ebm_buf[800 * 480];
    read(img_fd, ebm_buf, sizeof(ebm_buf));
    close(img_fd);

    int x, y;
    for (y = 0; y < 480; y++)
        for (x = 0; x < 800; x++)
            *(FB + x + 800 * y) = ebm_buf[x + 800 * y];

    return 0;
}

void show_color(int x_start, int y_start, int w, int h, unsigned int color)
{
    int x, y;
    for (y = y_start; y < y_start + h; y++)
        for (x = x_start; x < x_start + w; x++)
            *(FB + x + 800 * y) = color;
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