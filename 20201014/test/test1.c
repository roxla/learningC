#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

/*多种颜色循环切换，间隔一秒*/

int main()
{
    // 1.打开并映射
    int lcd_fd, i, j;                  //定义变量
    lcd_fd = open("/dev/fb0", O_RDWR); //打开LCD文件
    if (lcd_fd == -1)                  //判断lcd_fd是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    //映射显存
    unsigned int *FB;
    FB = mmap(
        NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
        800 * 480 * 4,          // 内存的大小（开发板屏幕4*800*480字节）
        PROT_READ | PROT_WRITE, // 保护权限（PROT_READ|PROT_WRITE）
        MAP_SHARED,             // 共享的	MAP_SHARED
        lcd_fd,                 // 映射的文件描述符
        0);                     // 映射后偏移量（一般填0，不偏移）

    // 2.操作显存，显示颜色
    unsigned int color_red = 0x00FF0000;
    for (i = 0; i < 480; i++)
    {
        for (j = 0; j < 800; j++)
        {
            *(FB + j + 800 * i) = color_red;
        }
    }

    // for(480次)
    // for(800次)

    // 3.取消映射并关闭LCD
    // munmap	//取消映射
    close(lcd_fd); //关闭文件

    return 0;
}