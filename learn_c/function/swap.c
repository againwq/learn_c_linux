#include<stdio.h>

void swap_num(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void get_array_size(int a[10]){
    printf("在函数里面%lu\n", sizeof(a)); //这里只是int的长度，证明了这里传入的只是数组第一个元素的地址
    //因此如果要传入数组，直接定义参数为int *a即可，不用指定长度
}

int main(int argc, char const *argv[])
{
    int a = 10, b = 20;
    int c[10];
    printf("main里面%lu\n", sizeof(c)); //这里才是数组的总长度（字节数）
    get_array_size(c);

    swap_num(&a, &b);
    printf("%d\n", a);
    printf("%d", b);

    return 0;
}
