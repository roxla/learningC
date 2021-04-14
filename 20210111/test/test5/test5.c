#include <stdio.h>

void main()
{
	int i = 1, mark = 1;
	double sum = 0;
	while (i <= 100)
	{
		sum += mark * (1.0 / i);
		mark = (-1)*mark;
		i++;
	}
	printf("%lf\n", sum);
}