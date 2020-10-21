#include <stdio.h>  
#include <sys/types.h>
#include <dirent.h>  
#include <string.h>  

int main(int argc, char const *argv[])
{
	// 1.打开目录
	DIR *dp;
	dp = opendir("/mnt/udisk/pic");
	if(dp == NULL)
	{
		perror("open dir failed");
		return 1;
	}

	// 2.读取目录项
	struct dirent *temp;
	int len;

	while(1)
	{
		temp = readdir(dp);
		// 1.判断是否读取完毕
		if(temp == NULL)	
			break;
		// 2.去除 "." 和 ".."
		// if(temp->d_name=="." || temp->d_name=="..")	//不允许比较！
		// if(strcmp(temp->d_name, ".")==0 || strcmp(temp->d_name, "..")==0)
		if(!strcmp(temp->d_name, ".") || !strcmp(temp->d_name, ".."))
			continue;
        printf("%s\n", temp->d_name);
		// 3.筛选.txt
		// len = strlen(temp->d_name);

		// if(len>4 && temp->d_name[len-4]=='.'
		// 		 && temp->d_name[len-3]=='e'
		// 		 && temp->d_name[len-2]=='b'
		// 		 && temp->d_name[len-1]=='m')
		// {
		// 	printf("%s\n", temp->d_name);
		// }

		// 012345678		长度
		// hello.txt		9
		// 1.txt			5
		// 123.txt			7
	}

	// 3.关闭目录
	closedir(dp);

	return 0;
}
