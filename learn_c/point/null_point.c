#include<stdio.h>

int main(int argc, char const *argv[])
{
    int *p1 = NULL;  //这是一种初始化变量的方法， 推荐
    int *p2;
    printf("%d", *p2); 
    printf("%d", *p1);  //对NULL指针进行*取值是非法的
    return 0;
}
