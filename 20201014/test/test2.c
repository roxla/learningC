#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// lcd文件全局变量
int lcd_fd;
// 映射内存地址全局变量
unsigned int *FB;

//打开lcd
int open_lcd(void);
// 显示颜色
void show_color(int x_start, int y_start, int w, int h, unsigned int color); //参数 X轴起始位置 Y轴起始位置 色块宽度 色块高度 颜色

int main()
{
    int lcd = 0;
    lcd = open_lcd();
    if (lcd != 0)
        return 0;

    int i, length, h;
    unsigned int arr[] = {0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000, 0x00FFFFFF, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000, 0x00FFFFFF};
    length = sizeof(arr) / sizeof(arr[0]);
    h = 480;
    while (1)
    {
        for (i = 0; i < length; i++)
        {
            show_color(800 / 2 - h / 2, 480 / 2 - h / 2, h, h, arr[i]);
            sleep(1);
            h = h - 80;
            if (h <= 0)
            {
                show_color(0, 0, 800, 480, 0x00000000);
                h = 480;
                i = -1;
            }
        }
    }

    // 3.取消映射并关闭LCD
    munmap(FB, 800 * 480 * 4); //取消映射
    close(lcd_fd);             //关闭文件
    return 0;
}

void show_color(int x_start, int y_start, int w, int h, unsigned int color)
{
    int x, y;
    for (y = y_start; y < y_start + h; y++)
        for (x = x_start; x < x_start + w; x++)
            *(FB + x + 800 * y) = color;
}

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