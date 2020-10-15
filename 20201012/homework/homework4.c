#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int lcd_fd, i, j;
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed");
        return 1;
    }
    unsigned int arr[] = {0x00FF8080, 0x008000FF, 0x007785FF};
    int length = sizeof(arr) / sizeof(arr[0]);
    for (j = 0; j < length; j++)
    {
        lseek(lcd_fd, 0, SEEK_SET);
        for (i = 0; i < 480 * 800; i++)
            write(lcd_fd, &arr[j], 4);
        sleep(5);
    }

    close(lcd_fd);

    return 0;
}