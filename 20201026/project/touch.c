#include <linux/input.h> //输入子系统头文件

#include "my.h"

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
    // 读取触摸屏数据（需要使用输入子系统对原始数据进行解析）
    struct input_event ts_buf;
    bool x_ready = false, y_ready = false;
    int pres = -1;

    while (1)
    {
        bzero(&ts_buf, sizeof(ts_buf));
        read(ts_fd, &ts_buf, sizeof(ts_buf));
        // printf("type:0x%x code:0x%x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);
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

// 获取xy坐标，滑动(通过地址传递，修改两个变量数据)
void get_xy_slide()
{
    // 读取触摸屏数据（需要使用输入子系统对原始数据进行解析）
    struct input_event ts_buf;
    bool x_ready1 = false, y_ready1 = false, x_ready2 = false, y_ready2 = false;
    int pres = -1, i = 0, x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    while (1)
    {
        bzero(&ts_buf, sizeof(ts_buf));
        read(ts_fd, &ts_buf, sizeof(ts_buf));
        i++;
        // printf("type:0x%x code:0x%x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);
        if (i <= 3)
        {
            if (ts_buf.type == EV_ABS) // 触摸屏事件
            {
                if (ts_buf.code == ABS_X) //X轴
                {
                    x1 = ts_buf.value;
                    x_ready1 = true;
                    y_ready1 = false; //确保先获取到的是x坐标
                }
                else if (ts_buf.code == ABS_Y) //Y轴
                {
                    y1 = ts_buf.value;
                    y_ready1 = true;
                }
            }
        }
        if (ts_buf.type == EV_ABS) // 触摸屏事件
        {
            if (ts_buf.code == ABS_X) //X轴
            {
                x2 = ts_buf.value;
                x_ready2 = true;
                y_ready2 = false; //确保先获取到的是x坐标
            }
            else if (ts_buf.code == ABS_Y) //Y轴
            {
                y2 = ts_buf.value;
                y_ready2 = true;
            }
        }

        if (ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH)
            pres = ts_buf.value;

        // 当获取到了x和y坐标
        if (x_ready1 && y_ready1 && x_ready2 && y_ready2 && pres == 1)
        {
            if (x2 > x1 && y2 > y1)
            {
                printf("1\n");
                i = 0;
                break;
            }
            else
            {
                printf("0\n");
                i = 0;
                break;
            }
        }
    }
}