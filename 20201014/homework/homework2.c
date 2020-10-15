#include <errno.h>
#include <fcntl.h>
#include <linux/input.h> //输入子系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    // 1.打开触摸屏设备文件	--->   “/dev/input/event0”
    int ts_fd, x, y;
    ts_fd = open("/dev/input/event0", O_RDONLY);
    if (ts_fd == -1)
    {
        perror("open ts failed");
        return 1;
    }

    // 2.读取触摸屏数据（需要使用输入子系统对原始数据进行解析）
    struct input_event ts_buf;

    while (1)
    {
        bzero(&ts_buf, sizeof(ts_buf));
        read(ts_fd, &ts_buf, sizeof(ts_buf));
        if (ts_buf.type == EV_ABS)
        {
            if (ts_buf.code == ABS_X)
                x = ts_buf.value;
            if (ts_buf.code == ABS_Y)
                y = ts_buf.value;
            x = x * 800 / 1024;
            y = y * 480 / 600;
            printf("(%d, %d)\n", x, y);
            x = 0;
            y = 0;
        }
        // printf("type:0x%x code:0x%x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);
    }

    return 0;
}
