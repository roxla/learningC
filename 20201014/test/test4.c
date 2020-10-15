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
void show_img(int x_start, int y_start, int w, int h, unsigned int *img); //参数 X轴起始位置 Y轴起始位置 色块宽度 色块高度 颜色
int open_img(char *fd_name);

int main()
{
    int lcd = 0, i, length;
    lcd = open_lcd();
    if (lcd != 0)
        return 0;

    char arr[][10] = {"1.ebm", "2.ebm", "3.ebm"};
    length = sizeof(arr) / sizeof(arr[0]);
    for (i = 0; i < length; i++)
    {
        open_img(arr[i]);
        sleep(5);
    }

    // 3.取消映射并关闭LCD
    munmap(FB, 800 * 480 * 4); //取消映射
    close(lcd_fd);             //关闭文件

    return 0;
}

void show_img(int x_start, int y_start, int w, int h, unsigned int *img)
{
    int x, y;
    for (y = y_start; y < y_start + h; y++)
        for (x = x_start; x < x_start + w; x++)
            *(FB + x + 800 * y) = img[x + 800 * y];
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

int open_img(char *fd_name)
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

    // 显示图片
    show_img(0, 0, 800, 480, ebm_buf);

    return 0;
}