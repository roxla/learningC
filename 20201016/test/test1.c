#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char filename[20] = "1.txt";

    struct stat buf;
    if (stat(filename, &buf) == -1)
    {
        perror("stat failed");
        return 1;
    }

    printf("st_dev: %lu\n", buf.st_dev);
    printf("st_ino: %lu\n", buf.st_ino);
    printf("st_mode: %d\n", buf.st_mode);
    printf("st_nlink: %lu\n", buf.st_nlink);
    printf("st_uid: %d\n", buf.st_uid);
    printf("st_gid: %d\n", buf.st_gid);
    printf("st_rdev: %lu\n", buf.st_rdev);
    printf("st_size: %ld\n", buf.st_size);
    printf("st_blksize: %ld\n", buf.st_blksize);
    printf("st_blocks: %ld\n", buf.st_blocks);
    printf("st_atime: %ld\n", buf.st_atime);
    printf("st_mtime: %ld\n", buf.st_mtime);
    printf("st_ctime: %ld\n", buf.st_ctime);

    return 0;
}
