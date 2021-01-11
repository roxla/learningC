#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define W 640
#define H 480
#define F 4
#define SIZE_W 800 // 屏幕高度
#define SIZE_H 480 // 屏幕高度

unsigned int *FB;
int lcd_fd;

// 自定义一个结构体存放每个缓冲块的首地址和大小
struct bufattr
{
    void *start;    // 缓冲块的首地址
    int somelength; // 缓冲块的大小
};

//封装一个函数把YUV转换成ARGB
int yuvtorgb(int y, int u, int v)
{
    unsigned int r, g, b;
    unsigned int pix;
    r = 1164 * (y - 16) / 1000 + 1596 * (v - 128) / 1000;
    g = 1164 * (y - 16) / 1000 + 813 * (v - 128) / 1000 - 391 * (u - 128) / 1000;
    b = 1164 * (y - 16) / 1000 + 2018 * (u - 128) / 1000;
    //修正运算结果
    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    pix = 0x00 << 24 | r << 16 | g << 8 | b;
    return pix;
}

//封装函数把一帧画面YUV数据转换成ARGB
//yuvdata --》你要转换的YUV
//argbdata --》存放转换得到的ARGB
int allyuvtorgb(char *yuvdata, int *argbdata)
{
    /*
		分析：yuvdata[0]   Y1
		      yuvdata[1]   U
			  yuvdata[2]   Y2
			  yuvdata[3]   V
        摄像头的YUV数据是按照：Y U Y V四个为一组(总共四个字节)
            比如：array[0].start  //第一个缓冲中的YUV画面数据
                array[0].start[0]  //第一个字节   Y1
                array[0].start[1]  //第二个字节   U
                array[0].start[2]  //第三个字节   Y2
                array[0].start[3]  //第四个字节   V
                array[0].start[4]  //第5个字节    Y1
                array[0].start[5]  //第6个字节    U
                array[0].start[6]  //第7个字节    Y2
                array[0].start[7]  //第8个字节    V
            注意：每组中两个Y的值是不同的
	*/
    int i, j;
    for (i = 0, j = 0; i < W * H; i + 2, j + 4)
    {
        // 每次循环获取两个像素点
        argbdata[i] = yuvtorgb(yuvdata[j], yuvdata[j + 1], yuvdata[j + 3]);
        argbdata[i + 1] = yuvtorgb(yuvdata[j + 2], yuvdata[j + 1], yuvdata[j + 3]);
    }
}

// 打开LCD
int open_lcd(void)
{
    lcd_fd = open("/dev/fb0", O_RDWR); //打开LCD文件
    if (lcd_fd == -1)                  //判断lcd_fd是否等于-1,等于（打开失败）
    {
        perror("open lcd failed"); //打开失败输出open failed
        return 1;
    }

    //映射显存
    FB = mmap(
        NULL,                   // 指定映射的地址（一般填NULL，由系统随机分配）
        SIZE_H * SIZE_W * 4,    // 内存的大小（开发板屏幕4*800*480字节）
        PROT_READ | PROT_WRITE, // 保护权限（PROT_READ|PROT_WRITE）
        MAP_SHARED,             // 共享的	MAP_SHARED
        lcd_fd,                 // 映射的文件描述符
        0);                     // 映射后偏移量（一般填0，不偏移）
    if (FB == MAP_FAILED)
    {
        perror("mmap failed");
        return -1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    open_lcd();
    int camerafd;
    int ret;
    // 打开摄像头的驱动 (/dev/video7)
    camerafd = open("/dev/video7", O_RDWR);
    // 设置摄像头采集画面的格式(画面宽，高，画面的格式等等)
    struct v4l2_format myfmt;
    bzero(&myfmt, sizeof(myfmt));
    myfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myfmt.fmt.pix.width = W;
    myfmt.fmt.pix.height = H;
    myfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    myfmt.fmt.pix.field = V4L2_FIELD_NONE;
    // 设置摄像头的采集格式
    ret = ioctl(camerafd, VIDIOC_S_FMT, &myfmt);
    if (ret == -1)
    {
        perror("设置摄像头的采集格式失败!\n");
        return -1;
    }
    // 跟摄像头的驱动申请缓冲块(用来存放等一会才采集的画面数据)
    struct v4l2_requestbuffers myreqbufs;
    bzero(&myreqbufs, sizeof(myreqbufs));
    myreqbufs.count = F;
    myreqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myreqbufs.memory = V4L2_MEMORY_MMAP;
    // 申请摄像头的缓冲块
    ret = ioctl(camerafd, VIDIOC_REQBUFS, &myreqbufs);
    if (ret == -1)
    {
        perror("申请缓冲块失败!\n");
        return -1;
    }
    // 分配刚才你申请的4个缓冲块
    // 定义结构体数组存放缓冲块的信息
    struct bufattr array[F];
    struct v4l2_buffer buf;
    int i;
    for (i = 0; i < F; i++)
    {
        bzero(&buf, sizeof(buf));
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(camerafd, VIDIOC_QUERYBUF, &buf);
        if (ret == -1)
        {
            perror("分配缓冲块失败!\n");
            return -1;
        }
        // 立马把分配的缓冲块做个映射
        array[i].somelength = buf.length;
        array[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camerafd, buf.m.offset);
        if (array[i].start == NULL)
        {
            perror("映射缓冲块首地址失败!\n");
            return -1;
        }
        //顺便入个队
        ret = ioctl(camerafd, VIDIOC_QBUF, &buf);
        if (ret == -1)
        {
            perror("入队失败!\n");
            return -1;
        }
    }
    // 启动摄像头采集
    enum v4l2_buf_type mytype;
    mytype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(camerafd, VIDIOC_STREAMON, &mytype);
    if (ret == -1)
    {
        perror("启动采集失败!\n");
        return -1;
    }
    //定义一个数组存放转换得到的一帧完整的ARGB数据
    int argbbuf[W * H]; //W*H
                        //让摄像头的画面出队(把画面数据从缓冲块中取出来)，入队循环进行，产生源源不断的视频流
    while (1)           //程序不要退出，摄像头不断的出队，入队画面数据
    {
        for (i = 0; i < F; i++)
        {
            bzero(&buf, sizeof(buf));
            buf.index = i; // 缓冲块的索引
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            //画面出队,把画面从缓冲块中取出来
            ret = ioctl(camerafd, VIDIOC_DQBUF, &buf); //出队
            if (ret == -1)
            {
                perror("出队失败!\n");
                return -1;
            }
            ret = ioctl(camerafd, VIDIOC_QBUF, &buf); //入队
            if (ret == -1)
            {
                perror("入队失败!\n");
                return -1;
            }
            //把出队的画面数据在开发板的液晶屏上显示出来
            /*
				出队的画面数据在哪里？
				   array[i].start这个首地址中就是画面数据(YUV)
				画面数据如何填充到液晶屏上显示呢？
				   第一步：mmap()液晶屏首地址--》int *lcdmem=mmap() 学习过
				   第二步：lcdmem+偏移=画面数据
				YUV格式的画面无法在液晶屏直接显示(原因是液晶屏支持的像素编码方式是ARGB)
				YUV数据如何转换成RGB、ARGB  --》已解决
			*/
            allyuvtorgb(array[i].start, argbbuf);
            //把转换得到的ARGB数据填充液晶屏
            int y;
            for (y = 0; y < H; y++)
                memcpy(FB + 800 * y, &argbbuf[W * y], W * 4);
        }
    }

    munmap(FB, SIZE_W * SIZE_H * 4); //取消映射
    close(lcd_fd);                   //关闭文件
    return 0;
}
