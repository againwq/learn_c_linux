#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    char a[] = "eeee";
    char b[] = "eeee";
    int c = strcmp(a, b);  //相等时返回0
    printf("%d", c);
    return 0;
}


