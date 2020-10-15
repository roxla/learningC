#include <stdio.h>

// int main(void)
// {
//     char s[] = "man";
//     int i;
//     for (i = 0; s[i]; i++)
//     {
//         printf("\n%c%c%c%c", s[i], *(s + i), *(i + s), i[s]);
//     }
// }

int main(void)
{
    int y = 9;
    for (; y > 0;
         y--)
    {
        if (y % 3 == 0)
            printf("%d", --y);
    }
}