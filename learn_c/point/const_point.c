#include<stdio.h>

int main(int argc, char const *argv[])
{
    int b = 20;
    const int a = 10;
    const int *p = &a;
    printf("%d\n", *p); //10
     //这是被禁止的，指针指向的值不能改*p = 20
    p = &b; //这是被允许的，指针本身还是变量             
    printf("%d\n", *p); // 20
    b = 30;
    printf("%d", *p);  //30
     /**如果p是 int * const p这样来定义，那么情况恰好相反，这时指针是常量指针
      * ，指针本身不能改，但是指向的值可以改
      * 如果是const int * const p,则指针本身和指针指向的值都不能被修改
     */
}