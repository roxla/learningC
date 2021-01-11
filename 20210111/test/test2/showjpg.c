#include <stdio.h>
#include "jpeglib.h"  //包含jpg库的头文件
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//把显示jpg的代码封装成函数
/*
	参数：jpgname --》jpg图片的路径名
		  x --》x坐标
		  y --》y坐标
*/
int showany_jpg(char *jpgname,int x,int y)
{
	int i,j;
	int lcdfd;
	int *lcdmem;
	//打开lcd
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		perror("打开lcd失败!\n");
		return -1;
	}
	
	//映射得到lcd的首地址
	lcdmem=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		perror("映射lcd失败!\n");
		return -1;
	}
	
	//定义解压缩结构体变量和保存出错信息的结构体变量,并初始化这两个变量
	struct jpeg_decompress_struct mydecom;
	jpeg_create_decompress(&mydecom);
	struct jpeg_error_mgr myerr;
	mydecom.err=jpeg_std_error(&myerr);
	

	
	//指定解压缩数据源
	FILE *myfile=fopen(jpgname,"r+");
	if(myfile==NULL)
	{
		perror("打开jpg图片失败!\n");
		return -1;
	}
	jpeg_stdio_src(&mydecom,myfile);
	
	//读取jpg图片的头信息
	jpeg_read_header(&mydecom,true);
	
	//开始解压缩
	jpeg_start_decompress(&mydecom);
	
	//打印图片的属性信息
	printf("图片原始宽：%d\n",mydecom.image_width);
	printf("图片原始高：%d\n",mydecom.image_height);
	
	printf("缩小以后图片宽：%d\n",mydecom.output_width);
	printf("缩小以后图片高：%d\n",mydecom.output_height);
	
	
	char *buf=malloc(mydecom.output_width*3);
	int otherbuf[mydecom.output_width]; //保存一行ARGB数据
	//读取你刚才解压的jpg数据，然后填充到开发板的液晶屏上
	for(i=0; i<mydecom.output_height; i++)
	{
		jpeg_read_scanlines(&mydecom,(JSAMPARRAY)&buf,1); //一次读取一行数据，存放到buf中
		//立马把读取到的一行RGB(ARGB)数据填充到液晶屏
		/*
			buf[0][1][2]三个字节RGB
			   [3][4][5]三个字节RGB
		*/
		for(j=0; j<mydecom.output_width; j++)
			otherbuf[j]=0x00<<24|buf[3*j]<<16|buf[3*j+1]<<8|buf[3*j+2];
		
		/*
			第一行：otherbuf[0]----otherbuf[mydecom.image_width-1]
			液晶屏：lcdmem
			第二行：otherbuf[0]----otherbuf[mydecom.image_width-1]  //由于otherbuf重复使用
			液晶屏：lcdmem+800
		*/
		//把转换的数据填充lcd中
		memcpy(lcdmem+800*(y+i)+x,otherbuf,mydecom.output_width*4);
	}
	 
	//收尾
	jpeg_finish_decompress(&mydecom);
	jpeg_destroy_decompress(&mydecom);
	fclose(myfile);
	munmap(lcdmem,800*480*4);
	free(buf);
	close(lcdfd);
	return 0;
}
int main(int argc,char **argv)
{
	showany_jpg(argv[1],200,200);
}