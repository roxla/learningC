#include "head.h"

int main()
{
	int fd;
	fd = open("/dev/input/event0",O_RDONLY);
	if(fd < 0)
		printf("open event0 error!\n");

	struct input_event buf;
	int x,y;
	while(1)
	{
		read(fd,&buf,sizeof(buf));

		//记录松手前x轴的坐标
		if(buf.type == EV_ABS && buf.code == ABS_X)
		{
			x = buf.value;
		}

		//记录松手前y轴的坐标
		if(buf.type == EV_ABS && buf.code == ABS_Y)
		{
			y = buf.value;
		}

		//判断什么时候松手
		if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)  //一旦松手，条件就会成立
		{
			//判断一下松手前的位置在哪里
			if(x > 512 && y < 300)
			{
				printf("右上方!\n");
			}

			if(x > 512 && y > 300)
			{
				printf("右下方!\n");
			}

			if(x < 512 && y < 300)
			{
				printf("左上方!\n");
			}

			if(x < 512 && y > 300)
			{
				printf("左下方!\n");
			}
		}
	}

	close(fd);

	return 0;
}