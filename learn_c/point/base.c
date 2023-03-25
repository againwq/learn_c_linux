#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = 10;
    int *p = &a;
    printf("%p\n", p);
    printf("%d\n", *p);  //通过指针访问a
    printf("%lu\n", sizeof(p)); //这里我的指针占8字节
    printf("%p\n", &p);   //指针也可以取地址

    *p = 15;   //通过指针给a重新赋值
    printf("%p\n", p); //赋值后a的地址、指针本身的地址都不会改变
    printf("%d\n", a);
    printf("%p", &p);
    return 0;
}
