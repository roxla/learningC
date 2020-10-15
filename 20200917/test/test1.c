#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned char a = 0x3A; // 0011 1010
    // ~(1 << 1) 1111 1101            
    printf("a: 0x%hhx\n", a & ~(1 << 1)); // 0011 1000

    return 0;
}
