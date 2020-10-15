#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// 全局变量
int lcd_fd;       // LCD设备文件描述符
unsigned int *FB; //显存首地址

// 打开lcd
int open_lcd(void);
// 关闭lcd
void close_lcd(void);
// 显示某个颜色色块
void show_color(int x_start, int y_start, int w, int h, unsigned int color);

int main(int argc, char const *argv[])
{
    // 把大象从冰箱A放到冰箱B，分几步？
    /************* 1.打开设备 **************/
    // 4.打开冰箱B门		--打开lcd		open_lcd
    open_lcd();

    /************* 2.程序逻辑 **************/
    // 1.打开冰箱A门		---打开图片文件
    int ebm_fd;
    ebm_fd = open("1.ebm", O_RDONLY);
    if (ebm_fd == -1)
    {
        perror("open ebm failed");
        return 1;
    }

    // 2.取出大象			---读取图片文件内容
    unsigned int ebm_buf[800 * 480] = {0};
    read(ebm_fd, ebm_buf, sizeof(ebm_buf));

    // 3.关闭冰箱A门		---关闭图片文件描述符
    close(ebm_fd);

    // 5.把大象放进去		--写入像素数据	指针操作
    // *(FB+0) = ebm_buf[0];
    // *(FB+1) = ebm_buf[1];
    // *(FB+2) = ebm_buf[2];
    // *(FB+3) = ebm_buf[3];

    int x, y;
    for (y = 0; y < 480; y++)
        for (x = 0; x < 800; x++)
            *(FB + x + 800 * y) = ebm_buf[x + 800 * y];

    /************* 3.关闭设备 **************/
    // 6.关闭冰箱B门		---关闭lcd		close_lcd
    close_lcd();

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
    FB = mmap(NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
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

// 关闭lcd
void close_lcd(void)
{
    // 3.取消映射并关闭LCD
    munmap(FB, 800 * 480 * 4); //取消映射
    close(lcd_fd);             //关闭文件
}

// 显示某个颜色色块
void show_color(int x_start, int y_start, int w, int h, unsigned int color)
{
    // 2.操作显存，显示颜色
    int x, y;
    for (y = y_start; y < y_start + h; y++)
        for (x = x_start; x < x_start + w; x++)
            *(FB + x + 800 * y) = color;
}
