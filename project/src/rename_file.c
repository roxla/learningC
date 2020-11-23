#include "myhead.h"

void *mytask(void *arg);
void file_rename(char *name);
int serch_file(char *name);

DIR *dp;

// 入口函数
void rename_file(char const *src_path)
{
	dp = opendir(src_path);
	if(dp == NULL)
	{
		printf("path error!\n");
		exit(0);
	}

    	struct dirent *temp = NULL;
    	// char temp_name[257] = {0};
	while(1)
	{	
		// 读取目录项
		temp = readdir(dp);
		// 判断是否读取完毕
        		if (temp == NULL)
        		{
            		break;
        		}

		if(strcmp(temp->d_name,".") == 0 || strcmp(temp->d_name,"..") == 0)
		{
			continue;
		}
		printf("read: %s\n", temp->d_name); // 读取顺序有问题,需要自己另做排序
		// 读取到给定的后缀的文件
		if (serch_file(temp->d_name))
		{
			// 线程池投放任务
			// printf("read: %s\n", temp->d_name);
			// strncpy(temp_name,temp->d_name,sizeof(temp_name));
			add_task(pool,mytask,(void *)temp->d_name);
		}
	}
}

void *mytask(void *arg)
{
	char *name = (char *)arg;
	// printf("mytask: %s\n", name);
	file_rename(name);
}

void file_rename(char *name)
{
	char new_name[257] = {0};
	snprintf(new_name,257,"%s_%d.%s",name_head,pool->num,name_type);
	// printf("%s %s\n", name,new_name);
	// rename(name,new_name);
}

// 文件类型检索
int serch_file(char *name)
{
    // 变量定义
    char temp[256];
    char *catalog;
    // 比较文件后缀，相同返回1，不同返回0
    strncpy(temp, name, sizeof(temp));
    cutOut(temp, ".", &catalog);
    if (!strcmp(catalog, name_type))
        return 1;
    else
        return 0;
}