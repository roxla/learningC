#include <stdio.h>

int main(void)
{
    float h, w, check1, check2;
    printf("请输入身高(m):");
    check1 = scanf("%f", &h);
    while (getchar() != '\n');
    printf("请输入体重(kg):");
    check2 = scanf("%f", &w);
    while (getchar() != '\n');
    if (check1 == 1 && check2 == 1)
    {
        if(h<3){
            float BMI = w / (h * h);
            printf("BMI值为%.1f\n", BMI);
            if(BMI>0 && BMI <= 18.4)
            {
                printf("体质为消瘦");
            }
            else if(BMI>=18.5 && BMI<=23.9)
            {
                printf("体质为正常");
            }
            else if(BMI>=24.0 && BMI<=27.9)
            {
                printf("体质为超重");
            }
            else if(BMI>=28.0)
            {
                printf("体质为肥胖");
            }
        }
        else
        {
            printf("请输入正确的身高\n");
        };
    }
    else
    {
        printf("请输入正确的数值\n");
    };
    getchar();
    return 0;
}