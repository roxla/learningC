#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

unsigned int len = 0;

/*
1.用户输入数据
2.将输入数据保存至 file_1.txt
3.用户输入复制数据长度
4.将用户指定长度的数据从 file_1.txt 复制到 file_2.txt
*/

int main(void)
{
    char file_name1[10] = {0};
    char file_name2[10] = {0};
    scanf("%s%s", file_name1, file_name2);

    // 1.打开两个文件
    // 待复制文件1（只读）
    int fd1;
    fd1 = open(file_name1, O_RDONLY);
    if (fd1 == -1)
    {
        perror("open file1 failed");
        return 1;
    }
    // 新文件2（复制文件。只写，不存在则创建，存在则清空）
    int fd2;
    fd2 = open(file_name2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd2 == -1)
    {
        perror("open file2 failed");
        return 2;
    }

    // 2. 读取文件1，写入到文件2。循环多次操作。
    char r_buf[100];
    int n_read;
    while (1)
    {
        bzero(r_buf, sizeof(r_buf));
        n_read = read(fd1, r_buf, sizeof(r_buf)); //读取100个字节（可能存在数据0('\0')）

        write(fd2, r_buf, n_read);

        if (n_read == 0) //如果成功读取到0个字节，说明已经读完该文件
        {
            printf("Done\n");
            break;
        }
    }

    //写入所有读取到的字节数（包括'\0'）

    // 3.关闭两个文件
    close(fd1);
    close(fd2);

    return 0;
}