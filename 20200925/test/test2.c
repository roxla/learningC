#include <stdio.h>

struct bookInfo
{
    unsigned int cat;
    unsigned short pig;
    char dog;
    char fox;
    union test1
    {
        int a;
        char b;
        double c;
    };
};

int main(int argc, char const *argv[])
{
    printf("%lu", sizeof(struct bookInfo));
    return 0;
}
