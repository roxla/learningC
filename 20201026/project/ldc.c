#include <sys/mman.h>

#include "my.h"

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