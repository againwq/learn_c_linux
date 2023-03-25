#include<stdio.h>

union Member
{
    int count;
    char status;
    int arr[5];
};

int main(int argc, char const *argv[])
{
    union Member member = {.count = 10}; //初始化共用体，和结构体类型
    member.status = '*';
    member.arr[0] = 10;
    member.count = 5;
    printf("%lu\n", sizeof(union Member));  //尺寸能够保存最大的那个成员的尺寸，但是不一定相等
    printf("%p\n", &member.status);   //三个内存地址相同
    printf("%p\n", &member.count);     //三个值不能同时被访问，最后被赋值的那个成员可以被访问
    printf("%p\n", member.arr);
    return 0;
}

