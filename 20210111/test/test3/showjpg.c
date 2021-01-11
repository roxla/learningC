#include <stdio.h>
#include "jpeglib.h" //包含jpg库的头文件
#include <sys/mman.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define W 800
#define H 480

int lcdfd;
int *lcdmem;

// 打开LCD
int lcd_open()
{
	lcdfd = open("/dev/fb0", O_RDWR);
	if (lcdfd == -1)
	{
		perror("打开lcd失败!\n");
		return -1;
	}

	// 映射得到lcd的首地址
	lcdmem = mmap(NULL, W * H * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcdfd, 0);
	if (lcdmem == NULL)
	{
		perror("映射lcd失败!\n");
		return -1;
	}
}

// 判断图片缩放尺寸，使其在LCD显示屏的分辨率内
int set_img_size(JDIMENSION width, JDIMENSION height)
{
	int temp = 0;
	if (width > W || height > H)
	{
		if (height > H)
		{
			temp = height / H;
		}
		else
		{
			temp = width / W;
		}
	}
	if (temp + 1 > 8)
		temp = 7;

	return temp + 1;
}

// 判断偏移的横坐标
int check_img_width(int w, JDIMENSION img_w)
{
	unsigned int temp;
	if (w + img_w > W)
	{
		temp = w + img_w - W;
		return w - temp;
	}
	else
	{
		return w;
	}
}

// 判断偏移的纵坐标
int check_img_height(int h, JDIMENSION img_h)
{
	unsigned int temp;
	if (h + img_h > H)
	{
		temp = h + img_h - H;
		return h - temp;
	}
	else
	{
		return h;
	}
}

int main(int argc, char const *argv[])
{
	int w = 0, h = 0;
	if (argv[2])
		w = atoi(argv[2]);
	if (argv[3])
		h = atoi(argv[3]);

	lcd_open();
	// 定义解压缩结构体变量和保存出错信息的结构体变量,并初始化这两个变量
	struct jpeg_decompress_struct mydecom;
	jpeg_create_decompress(&mydecom);
	struct jpeg_error_mgr myerr;
	mydecom.err = jpeg_std_error(&myerr);

	// 指定解压缩数据源
	FILE *myfile = fopen(argv[1], "r+");
	if (myfile == NULL)
	{
		perror("打开jpg图片失败!\n");
		return -1;
	}
	jpeg_stdio_src(&mydecom, myfile);

	// 读取jpg图片的头信息
	jpeg_read_header(&mydecom, true);

	//打印图片的属性信息
	printf("图片原始宽：%d\n", mydecom.image_width);
	printf("图片原始高：%d\n", mydecom.image_height);

	// 处理图片尺寸
	int size;
	JDIMENSION img_w, img_h;
	size = set_img_size(mydecom.image_width, mydecom.image_height);
	mydecom.scale_num = 1;
	mydecom.scale_denom = size;
	if (size > 1)
	{
		img_w = mydecom.output_width;
		img_h = mydecom.output_height;
	}
	else
	{
		img_w = mydecom.image_width;
		img_h = mydecom.image_height;
	}

	printf("缩小以后图片宽：%d\n", mydecom.output_width);
	printf("缩小以后图片高：%d\n", mydecom.output_height);

	//开始解压缩
	jpeg_start_decompress(&mydecom);

	char *buf = malloc(img_w * 3);
	int otherbuf[img_w]; //保存一行ARGB数据
	//读取你刚才解压的jpg数据，然后填充到开发板的液晶屏上
	int i, j;
	for (i = 0; i < img_h; i++)
	{
		jpeg_read_scanlines(&mydecom, (JSAMPARRAY)&buf, 1); //一次读取一行数据，存放到buf中
		//立马把读取到的一行RGB(ARGB)数据填充到液晶屏
		/*
			buf[0][1][2]三个字节RGB
			   [3][4][5]三个字节RGB
		*/
		for (j = 0; j < img_w; j++)
			otherbuf[j] = 0x00 << 24 | buf[3 * j] << 16 | buf[3 * j + 1] << 8 | buf[3 * j + 2];

		/*
			第一行：otherbuf[0]----otherbuf[mydecom.image_width-1]
			液晶屏：lcdmem
			第二行：otherbuf[0]----otherbuf[mydecom.image_width-1]  //由于otherbuf重复使用
			液晶屏：lcdmem+800
		*/
		//把转换的数据填充lcd中
		int x, y;
		x = check_img_width(w, img_w);
		y = check_img_height(h, img_h);
		memcpy(lcdmem + 800 * (y + i) + x, otherbuf, img_w * 4);
	}

	//收尾
	jpeg_finish_decompress(&mydecom);
	jpeg_destroy_decompress(&mydecom);
	fclose(myfile);
	munmap(lcdmem, W * H * 4);
	free(buf);
	close(lcdfd);
	return 0;
}
