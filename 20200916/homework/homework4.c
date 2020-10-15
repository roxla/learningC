#include <stdio.h>

int main(void)
{
    int days, weeks, inputDay;
    printf("please enter days: ");
    scanf("%d", &inputDay);
    while (getchar() != '\n')
        ;
    weeks = inputDay / 7;
    days = inputDay % 7;
    printf("%d days are %d weeks, %ddays.\n", inputDay, weeks, days);

    return 0;
}