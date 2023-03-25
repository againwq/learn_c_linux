#include<stdio.h>

typedef int (*PTR_TO_ARRAY)[10];     
typedef void (*Function)(void);
typedef int *(*PTR_TO_FUN_ARR)(int);  

void console_log(){
    printf("log\n");
}

int main(int argc, char const *argv[])
{
    int arr[10];
    PTR_TO_FUN_ARR func_arr[2] = { &console_log, &console_log, &console_log }; //定义一个用于存放函数地址的数组
    PTR_TO_ARRAY pa = &arr; //定义一个指向数组的指针
    Function func = &console_log;  //定义一个指向函数的指针
    printf("%d\n", (*pa)[0]);
    func();

    return 0;
}
