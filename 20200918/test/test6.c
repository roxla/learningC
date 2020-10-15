#include <stdio.h>

int main(void)
{
	int *ptr;
	int torf[2][2] = {12, 14, 16};
	ptr = torf[0];
	printf("%d\n", *ptr);
	printf("%d\n", *ptr+2);

	getchar();
	return 0;
}