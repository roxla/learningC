#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define COUNT 266

int main(void)
{
    int lcd_fd, i, j;
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed");
        return 1;
    }
    unsigned int color_1 = 0x00009900;
    unsigned int color_2 = 0x00FFFFFF;
    unsigned int color_3 = 0x00FF0000;
    for (i = 1; i <= 480; i++)
    {
        for (j = 1; j <= 266; j++)
        {
            write(lcd_fd, &color_1, 4);
        }
        for (j = 1; j <= 267; j++)
        {
            write(lcd_fd, &color_2, 4);
        }
        for (j = 1; j <= 267; j++)
        {
            write(lcd_fd, &color_3, 4);
        }
    }

    close(lcd_fd);

    return 0;
}