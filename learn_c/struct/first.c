#include<stdio.h>

struct Book
{
    char name[10];
    char author[10];
    float price;
};  
struct First
{
    char a;
    char b;
    int c;
} first;     //内存对齐,这个结构体占8字节
                             
int main(int argc, char const *argv[])
{
    struct Book book = {
        "binary",
        "xqc",
        12.9
    };
    struct Book book2 = {
        .name = "aesd",    //C99新增特性，给指定属性赋值
        .price = 10.8
    };
    printf("%s\n", book.name);
    printf("%s\n", book2.name);
    printf("sizeof float %lu", sizeof(first));  //8
}
