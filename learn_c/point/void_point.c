#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a = 10;
    int *pi = &a;
    char *ps = "awedsd";
    void *pv;

    pv = pi;
    printf("%p, %p\n", pv, pi);  //两者相等
    pv = ps;
    printf("%p, %p\n", pv, ps); //两者相等
    char *p;

    p = (char*) pv;   //void赋给其他时最好加上强转
    printf("%p", p);  //这时的p也没法进行取值操作了

    //注意，void指针无法取值

    return 0;
}
