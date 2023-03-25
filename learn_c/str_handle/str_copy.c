#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    char str1[] = "wadwawdffeeeeeeeeasd";
    char str2[] = "eeesdfff";
    char str3[300];
    strcpy(str1, str2);  //str1后面的部分存在但是不会被打印，因为把str2的copy过来会有一个结束符\0
    strncpy(str3, "awesddddfawe", 4);  //指定复制多少个字符

    printf("%s\n", str1);
    printf("%lu\n", sizeof(str1));  //通过sizeof可以获取到str1真正的长度
    printf("%s\n", str2);
    printf("%s\n", str3);
    return 0;
}
