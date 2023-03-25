#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
     //定义字符串
    char str[] = "ldsawdo";

    printf("%s\n", str);
    
    int length = strlen(str);    //strlen不包含 \0

    printf("%d\n", length);  
    printf("%lu", sizeof(str));  // 这个包含 \0

    return 0;
}

