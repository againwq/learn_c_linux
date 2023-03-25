#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    char a[] = "eeaasd";
    char b[] = "jiko895";
    char *p = strcat(a, b);
    //strncat(b, "868546", 2); //这个函数不会自动添加\0

    printf("%s\n", a);
    printf("%lu\n", sizeof(a));
    printf("%ld\n", strlen(a));
    printf("%s\n", b);
    printf("%lu\n", sizeof(b));//这里打印出

    printf("%s", p);
    return 0;
}
