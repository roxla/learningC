#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int fd1, fd2, fd3;
    char buf = "hello";
    fd1 = open("1.txt", O_RDWR | O_CREAT, 0751);
    fd2 = open("2.txt", O_RDWR | O_CREAT, 0751);
    write(fd1, buf, strlen(buf));
    fd3 = dup2(fd1, fd2);

    close(fd1);
    close(fd2);
    return 0;
}
