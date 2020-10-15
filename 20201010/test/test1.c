#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int file_fd;
    file_fd = open("1.txt", O_RDWR | O_CREAT | O_TRUNC, 0751);
    if (file_fd == -1)
    {
        perror("open failed");
        return 1;
    }
    printf("open successful!\n");

    char w_buf[20];
    bzero(w_buf, sizeof(w_buf));
    printf("Pls hello: ");
    scanf("%s", w_buf);
    while (getchar() != '\n')
        ;
    int n_write;
    n_write = write(file_fd, w_buf, strlen(w_buf));
    printf("%d writen\n", n_write);

    lseek(file_fd, 6, SEEK_SET);

    bzero(w_buf, sizeof(w_buf));
    printf("Pls name: ");
    scanf("%s", w_buf);
    while (getchar() != '\n')
        ;
    n_write = write(file_fd, w_buf, strlen(w_buf));
    printf("%d writen\n", n_write);

    lseek(file_fd, 0, SEEK_SET);

    char r_buf[20];
    bzero(r_buf, sizeof(r_buf));
    int n_read;
    n_read = read(file_fd, r_buf, sizeof(r_buf));
    printf("Get(%d): %s\n", n_read, r_buf);

    close(file_fd);
    return 0;
}
