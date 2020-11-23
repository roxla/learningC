#include "myhead.h"

// 全局变量
char name_head[50] = {0};
char name_type[10] = {0};
thread_pool *pool = NULL;

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		printf("argc error!\n");
		exit(0);
	}

	// 初始化线程池
	pool = malloc(sizeof(thread_pool));
	init_pool(pool,20);

	// 判断命名前缀
	if(!argv[3])
	{
		snprintf(name_head, 50, "%s", "a");
	}
	else
	{
		snprintf(name_head, 50, "%s", argv[3]);
	}

	// 判断文件类型
	if(!strncmp(argv[2],"dir",3))
	{
		printf("1\n");
	}
	else
	{
		snprintf(name_type, 10, "%s", argv[2]);
		rename_file(argv[1]);
	}

	// 销毁线程池
	destroy_pool(pool);

	return 0;
}

// int rename(const char *oldname, const char *newname);
// int chmod(const char *path, mode_t mode);
// int fchmodat(int fd, const char *path, mode_t mode, int flag);
// access()