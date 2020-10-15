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

int ts_fd; //触摸屏文件描述符

// 获取xy坐标(通过地址传递，修改两个变量数据)
void get_xy(int *x, int *y);

int main(int argc, char const *argv[])
{
    // 1.打开触摸屏设备文件	--->   “/dev/input/event0”
    ts_fd = open("/dev/input/event0", O_RDONLY);
    if (ts_fd == -1)
    {
        perror("open ts failed");
        return 1;
    }

    int pos_x, pos_y;
    while (1)
    {
        get_xy(&pos_x, &pos_y);
        printf("(%d, %d)\n", pos_x, pos_y);
    }

    // 3.关闭触摸屏设备文件
    close(ts_fd);

    return 0;
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
