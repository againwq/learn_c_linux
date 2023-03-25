#include<stdio.h>
int add(int, int);

inline int add(int a, int b){  //内联函数，编译时会直接将被调用的地方替换成整个函数（类似于宏定义），提高运行效率。现在的编译器会自动将一些函数变为内联函数
    return a + b;
}

int main(int argc, char const *argv[])
{
    int res = add(10,20);
    printf("%d", res);
    return 0;
}
