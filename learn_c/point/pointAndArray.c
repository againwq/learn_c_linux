#include<stdio.h>


/**
 * 数组名和指针的区别：指针是左值，修改，但是数组名只是指向第一个元素的地址，不能被修改
 *  char str[] = "nihao"; char *p = &str;
 * *(str + 1)和*(p + 1)都能访问到数组的第二个元素， p可以进行p++操作，但是str++操作是不被允许的
*/

int main(int argc, char const *argv[])
{
    char str[] = "eeawedsd";
    int count = 0;
    while(*str != "\0"){
        count ++;
        str ++;  //这里会报错，显示这个不是可修改的左值
    }
    printf("数组的长度为%d", count);
}

