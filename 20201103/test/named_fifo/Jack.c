#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	//1. 创建一个有名管道
	int ret;
	int fifo_fd;
	fifo_fd = open("/home/roxlas/fifo_test", O_RDWR);
	if (fifo_fd == -1)
	{
		ret = mkfifo("/home/roxlas/fifo_test", 0751);
		if (ret < 0)
			printf("mkfifo error!\n");
		fifo_fd = open("/home/roxlas/fifo_test", O_RDWR);
	}

	//2. 打开管道文件
	if (fifo_fd < 0)
		printf("open error!\n");

	//3. 将数据写入到文件中。
	char w_buf[50];
	while (1)
	{
		bzero(w_buf, sizeof(w_buf));
		fgets(w_buf, sizeof(w_buf), stdin);	  //从键盘获取数据，然后存放在w_buf。
		write(fifo_fd, w_buf, strlen(w_buf)); //将w_buf的数据写入到管道文件中。

		if (strncmp(w_buf, "I_Love_You", 10) == 0) //代表Jack发送了I_Love_You给Rose。
		{
			break;
		}
	}

	//4. 关闭文件
	close(fifo_fd);

	return 0;
}