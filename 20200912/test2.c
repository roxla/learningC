#include <stdio.h>

int main(void){
    char arr[] = "ahdoqbaofqbaiwfhnapfqphafpiqhfcaknvjbaofnkanfbaoiqnhnpfpqabjbdhfiwobgseioehpnanaoihebpajmapojwapoehgpnafpnsbghowobaojmf";
    char *pa = arr;
    int i = 0;
    while (1)
    {
        if(*(pa + i) == '\0')
        {
            break;
        }
        *(pa + i) += 1;
        i++;
    };
    printf("%s", pa);
    getchar();
    return 0;
}