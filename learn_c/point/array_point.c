//数组指针, 指向数组的指针
#include<stdio.h>

int main(int argc, char const *argv[])
{
    int temp[] = {1,2,3,4,5};
    int (*p)[5] = &temp;   
    printf("temp的首元素地址 %p\n", temp);
    printf("temp数组的地址 %p\n", &temp);
    printf("p的首元素 %p\n", p);    //p是temp这个数组的地址，值和temp（即第一个元素的地址）相同，但是含义不同
    printf("%p\n", *p);     //*p是temp[0]的地址，
    printf("%d\n", **p);    //**p是对temp[0]的数值
    printf("%p\n", (*p + 1));
    printf("%p\n", (temp + 1));
    return 0;
}
