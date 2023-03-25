#include<stdio.h>
#include<stdlib.h>

/**
 * malloc：从内存堆里面申请一块指定大小的内存，但是不会初始化
 * calloc： 申请并初始化内存
 * realloc:重新分配内存大小， 用于扩容
 * free：释放内存
 */
int main(int argc, char const *argv[])
{
    int *ptr;
    ptr = (int*)malloc(sizeof(int));  //malloc不会初始化内存, calloc函数可以帮你初始化内存
                                     //malloc返回一个void类型的指针，申请的内存在堆上，需要手动释放，否则直到程序结束才被释放
    if(ptr == NULL){                    //要注意申请完内存后不要直接改变指针的指向，这样之前申请的地址块就会丢失（也会导致内存泄漏）
        printf("内存分配失败");
        return 0;
    }else{
        scanf("%d", ptr);
        printf("%d\n", *ptr);
        free(ptr);   //释放内存
    }
    return 0;
}
