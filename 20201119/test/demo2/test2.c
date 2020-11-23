#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	fd_set set;
	//FD_SET(STDIN_FILENO,&set);

	int ret;
	ret = FD_ISSET(STDIN_FILENO,&set);
	printf("ret = %d\n",ret);

	return 0;
}