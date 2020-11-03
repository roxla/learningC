#include <stdio.h>
#include <stdlib.h>

struct Lnode /*以为当时做的题目没有告诉我是多少位的数字所以我选择了长度不受限的链栈*/
{
    int num;
    struct Lnode *next;
};

int Push(struct Lnode *top, int x) //压入数据
{
    struct Lnode *p = (struct Lnode *)malloc(sizeof(struct Lnode));
    p->num = x;
    p->next = top->next; //其实压入无非是链表的逆序输入，此为让新的节点指针域指向上一个节点
    top->next = p;
    return 0;
}

int Pop(struct Lnode *top, int *x) //*x是为了得到弹出的数据
{
    struct Lnode *q;
    q = top->next;
    if (top->next != NULL) //栈不为空的时候
    {
        *x = q->num;
        top->next = q->next;
        free(q); /*如果不释放q完全可以让p->next指向下一个，要注意的是这一步不可以跟上不颠倒，q毕竟只是指针，有指向的空间才有意义*/
        return 1;
    }
    else
        return 0;
}

void convey(struct Lnode *top, int a, int b)
{
    int t, i, *x, c;

    x = &c;
    while (a != 0)
    {
        t = a % b;
        Push(top, t);
        a = a / b;
    }

    i = 1;
    while (i)
    {
        i = Pop(top, x);
        if (i)
        {
            if (b == 16)
            {
                printf("%x", *x);
            }
            else
                printf("%d", *x);
        }
    }
    printf("\n");
}

int main()
{
    int a, b;
    struct Lnode *top;
    top = (struct Lnode *)malloc(sizeof(struct Lnode));
    top->next = NULL;
    printf("请输入十进制数: ");
    scanf("%d", &a);
    while (getchar() != '\n')
        ;
    printf("请输入转换进制: ");
    scanf("%d", &b);
    while (getchar() != '\n')
        ;

    convey(top, a, b);

    return 0;
}
