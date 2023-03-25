#include<stdio.h>
#include<stdarg.h>

int sum(int n, ...){  //定义可变参数
    int i, sum = 0;
    va_list vap;   //定义参数列表

    va_start(vap, n);  //指定要计算的数目
    for(i = 0; i < n; i++){
        sum += va_arg(vap, int); //对参数列表中的值累加
    }
    va_end(vap);//关闭参数列表

    return sum;
}

int main(int argc, char const *argv[])
{
    int res = sum(3,20,50,98);
    printf("%d", res);
    return 0;
}
