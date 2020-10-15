#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int lcd_fd, i;
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed");
        return 1;
    }
    unsigned int color_red = 0x00FF0000;
    for (i = 0; i < 480 * 800; i++)
        write(lcd_fd, &color_red, 4);
    close(lcd_fd);

    return 0;
}
