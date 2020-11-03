#include <stdio.h>
#include <stdlib.h>

// 链式队列节点结构体
typedef struct queue_node
{
    // 1.数据域
    int data;

    // 2.指针域
    struct queue_node *next;
} node_st, *node_pt;

// 链式队列管理结构体
typedef struct queue_list
{
    // A.队头节点位置（地址）
    struct queue_node *front;

    // B.队尾节点位置（地址）
    struct queue_node *rear;
} ql_st, *ql_pt;

// 初始化链式队列
ql_pt queue_list_init();

// 入队列
void en_queue(int en_data, ql_pt q);

// 出队列
void de_queue(int *temp_data, ql_pt q);

int main(int argc, char const *argv[])
{
    // 1.初始化一个空队列
    ql_pt q = queue_list_init();

    // 2.操作（正数入队列、负数出队列）
    int n;
    while (1)
    {
        fprintf(stderr, "Pls Input Dec: ");
        scanf("%d", &n);
        while (getchar() != '\n')
            ;

        if (n > 0)
        {
            // 入队列
            en_queue(n, q);
        }
        else
        {
            // 出队列
            // n = de_queue(q);	//也可以，返回值

            de_queue(&n, q); //地址传递，修改数据
            if (n > 0)
                printf("%d\n", n);
        }
    }

    return 0;
}

// 初始化链式队列
ql_pt queue_list_init(void)
{
    // A.给管理结构体分配一个堆空间
    ql_pt q = malloc(sizeof(ql_st));
    if (q == NULL)
    {
        perror("queue malloc failed");
        return NULL;
    }

    // B.让队头和队尾都指向NULL
    // q->front = q->rear = NULL;	//可以同时操作
    q->front = NULL;
    q->rear = NULL;

    // C.返回堆空间地址
    return q;
}

// 入队列
void en_queue(int en_data, ql_pt q)
{
    // A.给新节点分配堆空间，并把数据给入
    node_pt new_node = malloc(sizeof(node_st));
    if (new_node == NULL)
    {
        perror("new_node malloc failed");
        return;
    }
    new_node->data = en_data;

    // B.让新节点指针域，指向NULL
    new_node->next = NULL;

    // C.判断是否为第一个元素（当前队列为空队列）
    if (q->front == NULL)
    {
        // 是：让队头队尾指针都指向第一个新节点
        // q->front = q->rear = new_node;
        q->front = new_node;
        q->rear = new_node;
    }
    else
    {
        // 否：
        // 1.让队尾节点的指针域指向新节点
        q->rear->next = new_node;

        // 2.操作队尾指针rear，指向新节点
        q->rear = new_node;
    }
}

// 出队列
void de_queue(int *temp_data, ql_pt q)
{
    // （最后一个和前面的元素出队，也必须加以区分！）
    // A.判断是否为空队列
    if (q->rear == NULL)
    {
        return;
    }
    // B.获取队头节点的数据 temp_data
    *temp_data = q->front->data;
    // C.使用temp暂存一下队头节点的地址
    node_pt temp = q->front;
    // D.判断是否为最后一个元素（队头front、队尾rear相同指向）
    if (q->front == q->rear)
    {
        // 是：让队头队尾指针都指向NULL
        q->front = NULL;
        q->rear = NULL;
    }
    else
    {
        // 否：操作队头front，使其移动到 next即可
        q->front = q->front->next;
    }
    // E.释放堆空间free
    free(temp);
    // F.return返回队头节点的数据temp_data
    //（通过地址传递数据，无需返回）
}