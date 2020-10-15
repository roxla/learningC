#include<stdio.h>

int main(void)
{
    printf("char: %lu\n", sizeof(char));
    printf("int: %lu\n", sizeof(int));
    printf("float: %lu\n", sizeof(float));
    printf("double: %lu\n", sizeof(double));
    printf("long int: %lu\n", sizeof(long int));
    printf("long long int: %llu\n", sizeof(long long int));
    printf("short: %lu\n", sizeof(short));
    printf("unsigned int: %lu\n", sizeof(unsigned int));
    printf("unsigned long: %lu\n", sizeof(unsigned long));
    return 0;
}