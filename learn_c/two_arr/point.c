#include <stdio.h>

int main(int argc, char const *argv[])
{
    int array[2][3] = {{1,2,3}, {4,5,6}}; 
    int (*p)[3] = array;    //这里的array实际上就是第一行数组的指针
    printf("%d\n", **(p + 1)); //第二行第一列
    printf("%d\n", *(*(p + 1) + 1)); //第二行第二列
    printf("%d\n", *(*p + 1)); // 第一行第二列
    
    printf("%d\n", *(*p + 2)); // 第一行第三列
    printf("%d\n", *(*array + 1));
    return 0;
}
