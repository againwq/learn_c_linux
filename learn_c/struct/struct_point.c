#include<stdio.h>
#include<stdlib.h>
struct Date
{
    int year;
    int month;
    int day;
} date = {
    2023, 3, 17
};


int main(int argc, char const *argv[])
{
    struct Date *today; //定义一个结构体指针，但是程序分配的栈空间不足以容纳结构体
    printf("%lu\n", sizeof(today)); //这里是 8
    struct Date *tomorrow = (struct Date*)malloc(sizeof(struct Date)); //给没有初始化的结构体指针分配充分的堆空间

    tomorrow -> day = 20; // 有了足够的内存空间就可以赋值了

    today = &date;  //没有初始化的结构体指针不能通过 today->day来赋值，因为分配的内存空间不够

    printf("%d年%d月%d日\n", today->year, (*today).month, today->day);  //两种取值方法
    printf("%d", today -> day);
 
    return 0;
}
