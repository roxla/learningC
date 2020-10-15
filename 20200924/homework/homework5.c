#include <stdio.h>

struct Date
{
    int year;
    int month;
    int day;
};

int main(void)
{
    struct Date theDay = {
        year : 2020,
        month : 9,
        day : 24
    };
    int day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int allDay, i;
    allDay = theDay.day;
    for (i = 0; i < theDay.month - 1; i++)
        allDay += day[i];
    if (((theDay.year % 4 == 0 && theDay.year % 100 != 0) || theDay.year % 400 == 0) && theDay.month > 2)
        allDay += 1;
    printf("%d\n", allDay);

    return 0;
}
