#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    unsigned int *ebm_buf1 = calloc(800 * 480, sizeof(int));
    unsigned int ebm_buf2[800 * 480];
    printf("%ld\n", sizeof(*ebm_buf1));
    printf("%ld\n", sizeof(ebm_buf2));
    printf("%ld\n", sizeof(int));
    printf("%d\n", 5 / 2.5);

    return 0;
}
