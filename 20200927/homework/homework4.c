// 大端序(Big-endian)：高位字节存入低地址，低位字节存入高地址
// 小端序(Little-endian)：低位字节存入低地址，高位字节存入高地址
#include <stdio.h>

// 联合体union的存放顺序是所有成员都从低地址开始存放
union node
{
    int i;
    char a;
};

int main(void)
{
    union node num = {0};
    num.i = 1;
    int k = 0;
    k = (num.a) ? 1 : 0; // 若为小端模式，则a应该是1，表达式返回1；
    printf("%d", k);

    return 0;
}
