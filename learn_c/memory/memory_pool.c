#include<stdio.h>
#define MAX_COUNT 1024 //定义内存池最大数量
struct Node
{
    int value;
    struct Node *next;
};

struct Node *pool = NULL;    //定义内存池，使用典型的链表结构
//内存池可以减少malloc和free操作产生的内存碎片，如果程序中一个内存需要释放，可以考虑放在内存池中，如果要申请，就直接在内存池中去申请废弃的内存池