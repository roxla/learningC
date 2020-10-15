#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define COUNT 480 * 800 / 3

int main(void)
{
    int lcd_fd, i;
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed");
        return 1;
    }
    unsigned int color_1 = 0x001C1C1A;
    unsigned int color_2 = 0x00D4151A;
    unsigned int color_3 = 0x00FFFFFF;
    for (i = 0; i < 480 * 800; i++)
    {
        if (i >= 480 * 800 / 3 && i < 480 * 800 / 3 * 2)
        {
            write(lcd_fd, &color_2, 4);
        }
        if (i >= 480 * 800 / 3 * 2)
        {
            // printf("%d\n", i);
            write(lcd_fd, &color_3, 4);
        }
        // else if (i > 480 * 800 / 3 * 2)
        // {
        //     write(lcd_fd, &color_3, 4);
        // }
        write(lcd_fd, &color_1, 4);
    }
    // for (i = 0; i < COUNT; i++)
    // {
    //     write(lcd_fd, &color_1, 4);
    // }
    // for (i = 0; i < COUNT; i++)
    // {
    //     write(lcd_fd, &color_2, 4);
    // }
    // for (i = 0; i < COUNT; i++)
    // {
    //     write(lcd_fd, &color_3, 4);
    // }

    close(lcd_fd);

    return 0;
}