#include <stdio.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// 实现 ls -l 类似效果
// argc: 主函数参数个数
// argv: 指针数组（参数列表）
int main(int argc, char const *argv[])
{
	// char filename[20];
	// bzero(filename, sizeof(filename));
	// fgets(filename, sizeof(filename), stdin); strtok(filename, "\n");

	// printf("argv[0]: %s\n", argv[0]);
	// printf("argv[1]: %s\n", argv[1]);
	// printf("argv[2]: %s\n", argv[2]);

	// 获取属性
	struct stat buf;
	if(stat(argv[1], &buf) == -1)
	{
		perror("stat failed");
		return 1;
	}

/*
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
*/
	// 1.文件类型(st_mode的高4位)
	// printf("st_mode: %d\n", buf.st_mode);
	// st_mode:	xxxx yyyz zzzz zzzz
	// 		&	1111 0000 0000 0000
	// 	--->	xxxx 0000 0000 0000
	// 			1100 0000 0000 0000
	/*if((buf.st_mode&S_IFMT) == S_IFSOCK)
		printf("s");
	else if((buf.st_mode&S_IFMT) == S_IFLNK)
		printf("l");
	else if((buf.st_mode&S_IFMT) == S_IFREG)
		printf("-");
	else if((buf.st_mode&S_IFMT) == S_IFDIR)
		printf("d");*/
	//....
	// 尝试修改为 switch-case结构
	switch(buf.st_mode&S_IFMT)
	{
		case S_IFSOCK:
		printf("s"); break;
		case S_IFLNK:
		printf("l"); break;
		case S_IFREG:
		printf("-"); break;
		case S_IFDIR:
		printf("d"); break;
		case S_IFBLK:
		printf("b"); break;
		case S_IFCHR:
		printf("c"); break;
		case S_IFIFO:
		printf("p"); break;
	}
	// 2.文件权限(st_mode的低9位)
	// 使用位操作，将所有权限打印出来

	// st_mode:	xxxx yyyz zzzz zzzz
	// 		&	0000 0000 0010 0000
	char rwx[] = {'r', 'w', 'x'};
			//		0	1	 2
	int i;				
	for(i=0; i<9; i++)
		printf("%c", buf.st_mode&(0x0100>>i) ? rwx[i%3] : '-');
	printf(" ");

/*
	printf("%c", buf.st_mode&(0x0100>>0) ? rwx[0] : '-');	
	printf("%c", buf.st_mode&(0x0100>>1) ? rwx[1] : '-');	
	printf("%c", buf.st_mode&(0x0100>>2) ? rwx[2] : '-');	
	printf("%c", buf.st_mode&(0x0100>>3) ? rwx[0] : '-');	
	printf("%c", buf.st_mode&(0x0100>>4) ? rwx[1] : '-');			
*/


/*
	int i;
	for(i=0; i<3; i++)	// 0 1 2
	{
		if(buf.st_mode&(0x0100>>(3*i)))
			printf("r");
		else
			printf("-");

		if(buf.st_mode&(0x0100>>(3*i+1)))
			printf("w");
		else
			printf("-");

		if(buf.st_mode&(0x0100>>(3*i+2)))
			printf("x");
		else
			printf("-");
	}
	printf(" ");
*/

	// 3.硬链接数
	printf("%lu ", buf.st_nlink);

	// 4.用户名
	// printf("st_uid: %d\n", buf.st_uid);
	// 使用到  getpwuid 函数
	// struct passwd *passwd;
	// passwd = getpwuid(buf.st_uid);
	// printf ("%s ", passwd->pw_name);

	printf ("%s ", getpwuid(buf.st_uid)->pw_name);


	// 5.组名
	// printf("st_gid: %d\n", buf.st_gid);
	// 使用到  getgrgid函数
	// struct group *group;
	// group = getgrgid(buf.st_gid);
	// printf ("%s ", group->gr_name);

	printf ("%s ", getgrgid(buf.st_gid)->gr_name);

	// 6.文件大小（单位：字节）
	// （后续自己完成：设备文件：打印主次设备号		其他文件：打印文件大小）
	printf("%ld ", buf.st_size);

	// 7.最后属性更改时间(使用ctime)
	// printf("%ld ", buf.st_ctime);
	printf("%s ", strtok(ctime(&buf.st_ctime), "\n"));



	// 8.文件名
	printf("%s\n", argv[1]);

	return 0;
}
